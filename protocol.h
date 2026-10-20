#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QByteArray>
#include <QMetaType>
#include <QString>

/*
 * 通信逻辑（PLC与上位机基于modbus协议示例-20261008.xlsx）
 *
 * 传输：Modbus TCP 保持寄存器。表内地址写成 4x42000 这类形式，程序把 42000
 * 当作 PDU 地址发出。每组固定 32 字节，折成 16 个寄存器，寄存器内高字节在前。
 * 表的规则是：奇数字节为高字节，偶数字节为低字节。下面的“字节 N”都是表里的
 * 1 起编号，代码里的下标是 N-1。
 *
 * 上报（上位机只读。协议表写 PLC 每 1 秒写入；当前上位机每 3 秒读一次。包序号在字节 4）：
 *   0x01  42000  健康状态
 *   0x02  42100  伺服力矩、坐标、时分秒
 *   0x03  42200  电池。本表只写“详见 Word”，程序不解析载荷。
 * 连续两次及以上读到同一包序号，记一条丢包日志（时间、客户端名称、说明）。
 *
 * 命令（上位机写同一块寄存器，PLC 把回复填回这一块）：
 *   0x04  42300  舱门。字节 4：01 打开，02 关闭。字节 6 飞机型号，01 为 Q20。
 *   0x05  42400  前后归正。字节 4：01 张开，02 夹紧。
 *   0x06  42500  左右（充电）归正。同上。
 *   0x07  42600  字节 4 复位，6 设备风扇，8 电控柜风扇，10 空调，12 充电器电源，14 二维码灯。
 *   0x08  42700  字节 4 MQTT，6 风速雨雪，8 温湿度，10 UPS，12 充电器链路。01 正常，02 断联。
 * 回复不看字节 3：字节 28 为 02 执行中 / 03 拒绝 / 80 成功 / 81 失败，
 * 字节 30 为告警码，字节 32 为故障码。
 *
 * 《控制流程细节0810》把舱门写成 5 字节“01 04 01 序号 操作码”，回复状态放在字节 3。
 * 与接口表的列号冲突。发送以接口表为准；解析时若字节 28 没有状态、字节 3 却出现
 * 02/03/80/81，仍接受该结果，并在日志里写明“回复落在字节 3，与流程表一致”。
 *
 * 舱门操作码以接口协议表为准：01 打开舱门，02 关闭舱门，不按有无飞机拆开。
 * 《控制流程细节0810》里的 01/02/03/04 是旧流程，发送时不再使用。
 */

namespace Protocol {

constexpr int kFrameBytes = 32;
constexpr int kRegisterCount = 16;
constexpr quint8 kDeviceType = 0x01;
constexpr const char *kClientName = "上位机";

constexpr quint8 kCmdHealth = 0x01;
constexpr quint8 kCmdServo = 0x02;
constexpr quint8 kCmdBattery = 0x03;
constexpr quint8 kCmdDoor = 0x04;
constexpr quint8 kCmdGripFr = 0x05;
constexpr quint8 kCmdGripCh = 0x06;
constexpr quint8 kCmdPlcCtrl = 0x07;
constexpr quint8 kCmdNetwork = 0x08;

constexpr int kAddrHealth = 42000;
constexpr int kAddrServo = 42100;
constexpr int kAddrBattery = 42200;
constexpr int kAddrDoor = 42300;
constexpr int kAddrGripFr = 42400;
constexpr int kAddrGripCh = 42500;
constexpr int kAddrPlcCtrl = 42600;
constexpr int kAddrNetwork = 42700;

constexpr quint8 kStatusExecuting = 0x02;
constexpr quint8 kStatusRejected = 0x03;
constexpr quint8 kStatusOk = 0x80;
constexpr quint8 kStatusFail = 0x81;

constexpr quint8 kOpOn = 0x01;
constexpr quint8 kOpOff = 0x02;
constexpr quint8 kLinkUp = 0x01;
constexpr quint8 kLinkDown = 0x02;

constexpr quint8 kDoorOpen = 0x01;
constexpr quint8 kDoorClose = 0x02;
constexpr quint8 kGripOpen = 0x01;
constexpr quint8 kGripClose = 0x02;
constexpr quint8 kPlaneModelQ20 = 0x01;

// 0x07 控制项序号，对应字节 4、6、8、10、12、14
constexpr int kCtrlReset = 0;
constexpr int kCtrlDeviceFan = 1;
constexpr int kCtrlCabinetFan = 2;
constexpr int kCtrlAirCondition = 3;
constexpr int kCtrlChargerPower = 4;
constexpr int kCtrlQrLight = 5;
constexpr int kCtrlCount = 6;

// 0x08 链路序号，对应字节 4、6、8、10、12
constexpr int kNetMqtt = 0;
constexpr int kNetWindRain = 1;
constexpr int kNetTempHum = 2;
constexpr int kNetUps = 3;
constexpr int kNetCharger = 4;
constexpr int kNetCount = 5;

enum LogLevel {
    LogTrace = 0,
    LogInfo = 1,
    LogWarn = 2,
    LogError = 3
};

struct HealthStatus {
    quint8 seq = 0;
    quint8 plcState = 0;
    quint8 paddleFault = 0;
    quint16 topDoorFault = 0;
    quint16 liftFault = 0;
    quint16 gripFrFault = 0;
    quint16 gripChFault = 0;
    quint16 alarmRaw = 0;
    quint8 plcFault = 0;
    quint8 mechanism = 0;
    quint8 sensorA = 0;
    quint8 sensorB = 0;
    quint8 inputs = 0;
    quint8 outputs = 0;
    bool doorOpen = false;
    bool doorClose = false;
    bool gripFrOpen = false;
    bool gripFrClose = false;
    bool gripChOpen = false;
    bool gripChClose = false;
    bool paddleAtB = false;
    bool paddleAtA = false;
    bool smoke = false;
    bool planePresent = false;
    bool automatic = false;
    bool estop = false;
    bool qrLight = false;
    bool chargerPower = false;
    bool airCondition = false;
    bool cabinetFan = false;
    bool deviceFan = false;
    bool resetDone = false;
};

struct ServoStatus {
    quint8 seq = 0;
    quint8 hour = 0;
    quint8 minute = 0;
    quint8 second = 0;
    qint16 torqueTopDoor = 0;
    qint16 torqueLift = 0;
    qint16 torqueGripFr = 0;
    qint16 torqueGripCh = 0;
    qint16 posTopDoor = 0;
    qint16 posLift = 0;
    qint16 posGripFr = 0;
    qint16 posGripCh = 0;
};

struct CommandReply {
    bool recognized = false;
    bool statusAtByte28 = false;
    quint8 status = 0;
    quint8 alarm = 0;
    quint8 fault = 0;
    QString summary;
};

QString hexBytes(const QByteArray &frame);
quint16 readBe16(const QByteArray &frame, int index);
qint16 readBeS16(const QByteArray &frame, int index);

int addressOf(quint8 cmd);
QString commandName(quint8 cmd);
QString replyStatusText(quint8 status);
QString plcStateText(quint8 state);
QString doorOpcodeText(quint8 opcode);
QString gripOpcodeText(quint8 opcode);
QString controlItemName(int index);
QString networkItemName(int index);
QString driverFaultText(quint16 code);
QString processFaultText(quint8 code);
QString alarmText(quint8 code);

QByteArray blankFrame(quint8 cmd);
QByteArray buildMotionFrame(quint8 cmd, quint8 opcode, quint8 planeModel);
void setControlPair(QByteArray &frame, int itemIndex, quint8 opcode);

bool parseHealth(const QByteArray &frame, HealthStatus *out, QString *error);
bool parseServo(const QByteArray &frame, ServoStatus *out, QString *error);
CommandReply parseCommandReply(const QByteArray &frame);

QString describeHealth(const HealthStatus &s);
QString describeServo(const ServoStatus &s);
QString describeTxFrame(quint8 cmd, const QByteArray &frame);

// 启动时核对字节位置，避免以后改常量时把操作码写错位。
bool selfCheck(QString *error);

} // namespace Protocol

Q_DECLARE_METATYPE(Protocol::HealthStatus)
Q_DECLARE_METATYPE(Protocol::ServoStatus)
Q_DECLARE_METATYPE(Protocol::CommandReply)

#endif // PROTOCOL_H
