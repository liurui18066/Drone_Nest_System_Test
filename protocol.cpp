#include "protocol.h"

#include <QStringList>

namespace Protocol {

namespace {

struct CodeName {
    quint16 code;
    const char *name;
};

// 驱动器故障码表。数值等于表中 Er 号，16 位，高字节在前。
const CodeName kDriverFaults[] = {
    {0x0090, "Er090 FPGA通讯错误"},
    {0x00A0, "Er0A0 电流检测回路错误"},
    {0x00A1, "Er0A1 电流检测回路错误"},
    {0x00A3, "Er0A3 UVW断线"},
    {0x00A5, "Er0A5 直流母线回路错误"},
    {0x00A6, "Er0A6 温度检测回路错误"},
    {0x00B0, "Er0B0 控制电源电压过低"},
    {0x00B1, "Er0B1 控制电源电压过高"},
    {0x00B2, "Er0B2 控制电掉电"},
    {0x00C0, "Er0C0 直流母线电压过高"},
    {0x00D0, "Er0D0 直流母线电压过低"},
    {0x00D1, "Er0D1 主电输入缺相"},
    {0x00D2, "Er0D2 主电源断电"},
    {0x00E0, "Er0E0 软件过电流"},
    {0x00E1, "Er0E1 智能功率模块过流"},
    {0x00E2, "Er0E2 输出对地短路"},
    {0x00E4, "Er0E4 相电流过流"},
    {0x00F0, "Er0F0 驱动器过热"},
    {0x0100, "Er100 电机过载"},
    {0x0101, "Er101 驱动器过载"},
    {0x0102, "Er102 电机堵转故障"},
    {0x0103, "Er103 电机碰撞故障"},
    {0x0104, "Er104 低速时驱动器输出过载"},
    {0x0120, "Er120 电阻泄放回路过载"},
    {0x0122, "Er122 泄放电阻阻值设置过小"},
    {0x0123, "Er123 泄放过流"},
    {0x0150, "Er150 编码器断线"},
    {0x0151, "Er151 编码器通信错误"},
    {0x0152, "Er152 编码器匹配错误"},
    {0x0153, "Er153 多圈编码器电池故障"},
    {0x0154, "Er154 编码器参数下载故障"},
    {0x0155, "Er155 多圈编码器数据计数跳变"},
    {0x0156, "Er156 编码器过热故障"},
    {0x0157, "Er157 多圈编码器数据计数溢出"},
    {0x0170, "Er170 编码器数据出错"},
    {0x0171, "Er171 编码器参数读取错误"},
    {0x0180, "Er180 位置误差过大"},
    {0x0181, "Er181 速度误差过大"},
    {0x0182, "Er182 指令位置溢出"},
    {0x0190, "Er190 混合误差过大"},
    {0x01A0, "Er1A0 超速"},
    {0x01A1, "Er1A1 电机速度失控"},
    {0x01B0, "Er1B0 脉冲输入频率错误或总线指令抖动过大"},
    {0x01B1, "Er1B1 电子齿轮比错误"},
    {0x01B2, "Er1B2 单圈绝对值超过上下限"},
    {0x01B6, "Er1B6 PR模式电子齿轮比错误"},
    {0x01B7, "Er1B7 指令溢出"},
    {0x01B8, "Er1B8 绝对式多圈数据设置超限"},
    {0x01B9, "Er1B9 原点偏置设定错误"},
    {0x01C0, "Er1C0 两路STO同时触发"},
    {0x01C1, "Er1C1 第1路STO触发"},
    {0x01C2, "Er1C2 第2路STO触发"},
    {0x01C3, "Er1C3 STO供电1异常"},
    {0x01C4, "Er1C4 STO供电2异常"},
    {0x01C5, "Er1C5 STO输入电路1异常"},
    {0x01C6, "Er1C6 STO输入电路2异常"},
    {0x01C7, "Er1C7 STO电路BUFFER1异常"},
    {0x01C8, "Er1C8 STO电路BUFFER2异常"},
    {0x01C9, "Er1C9 STO软件流程监控异常"},
    {0x0210, "Er210 I/O输入端口分配重复"},
    {0x0211, "Er211 I/O输入端口功能设定错误"},
    {0x0212, "Er212 I/O输出端口功能设定错误"},
    {0x0240, "Er240 EEPROM参数CRC错误"},
    {0x0241, "Er241 EEPROM存储器错误"},
    {0x0243, "Er243 EEPROM保存厂商参数错误"},
    {0x0244, "Er244 EEPROM保存通信参数错误"},
    {0x0245, "Er245 EEPROM保存设备参数错误"},
    {0x0246, "Er246 EEPROM断电信息错误"},
    {0x0260, "Er260 脉冲再生限界保护"},
    {0x0280, "Er280 强制报警输入有效"},
    {0x05F0, "Er5F0 电机代码错误"},
    {0x05F1, "Er5F1 驱动器片选识别异常"},
    {0x05F3, "Er5F3 驱动功率选择故障"},
    {0x05F4, "Er5F4 电机电压等级不匹配"},
    {0x05F6, "Er5F6 驱动器与电机型号不匹配"},
    {0x0600, "Er600 内部错误"},
    {0x0601, "Er601 内部时序错误"},
    {0x0602, "Er602 中断超时故障"},
    {0x0603, "Er603 双核交互中断丢失"},
    {0x0604, "Er604 内部存储异常"},
    {0x0700, "Er700 加密不匹配"},
    {0x073A, "Er73A PDI中断丢失次数过多"},
    {0x073B, "Er73B DC中断丢失次数过多"},
    {0x073C, "Er73C DC周期漂移过大"},
    {0x0801, "Er801 EtherCAT通信未知错误"},
    {0x0802, "Er802 内存溢出"},
    {0x0803, "Er803 RAM越界"},
    {0x0805, "Er805 协议栈初始化失败"},
    {0x0806, "Er806 固件与XML或产品不匹配"},
    {0x0807, "Er807 PDO映射对象不存在"},
    {0x0808, "Er808 PDO映射对象长度不符"},
    {0x0809, "Er809 PDO映射对象属性错误"},
    {0x0811, "Er811 无效的ESM状态转换请求"},
    {0x0812, "Er812 未知的ESM状态转换请求"},
    {0x0813, "Er813 引导状态请求保护"},
    {0x0814, "Er814 固件无效"},
    {0x0815, "Er815 引导状态邮箱配置无效"},
    {0x0816, "Er816 预操作状态邮箱配置无效"},
    {0x0817, "Er817 无效SM配置"},
    {0x0818, "Er818 TPDO刷新超时"},
    {0x0819, "Er819 RPDO刷新超时"},
    {0x081A, "Er81A 同步丢失错误"},
    {0x081B, "Er81B PDI看门狗超时"},
    {0x081C, "Er81C 无效的同步管理器类型"},
    {0x081D, "Er81D 无效的输出配置"},
    {0x081E, "Er81E 无效的输入配置"},
    {0x081F, "Er81F 无效的看门狗配置"},
    {0x0821, "Er821 等待ESM初始状态"},
    {0x0822, "Er822 等待ESM预操作状态"},
    {0x0823, "Er823 等待主站安全操作请求"},
    {0x0824, "Er824 无效的TPDO映射"},
    {0x0825, "Er825 无效的RPDO映射"},
    {0x0828, "Er828 不支持同步模式"},
    {0x082B, "Er82B TPDO/RPDO刷新超时"},
    {0x082C, "Er82C 致命同步错误"},
    {0x082D, "Er82D 无同步错误"},
    {0x082E, "Er82E 同步周期设置过小"},
    {0x0830, "Er830 无效DC配置"},
    {0x0832, "Er832 DC周期漂移过大"},
    {0x0833, "Er833 DC中断丢失次数过多"},
    {0x0834, "Er834 PDI中断丢失次数过多"},
    {0x0835, "Er835 无效的DC周期"},
    {0x0836, "Er836 无效的DC同步周期"},
    {0x0838, "Er838 同步错误"},
    {0x0850, "Er850 ESC EEPROM访问错误"},
    {0x0851, "Er851 ESC EEPROM错误"},
    {0x0852, "Er852 ECAT物理链路故障"},
    {0x0860, "Er860 硬件帧丢失错误"},
    {0x0870, "Er870 当前模式不支持"},
    {0x0871, "Er871 当前同步类型下不支持该模式"},
    {0x0890, "Er890 回零错误"},
};

const CodeName kProcessFaults[] = {
    {0x01, "急停被拍下"},
    {0x02, "烟雾报警器报警"},
    {0x03, "顶门轴控报警"},
    {0x04, "升降轴控报警"},
    {0x05, "前后归正轴控报警"},
    {0x06, "左右归正轴控报警"},
    {0x0A, "整机复位指令超时"},
    {0x0B, "舱门关闭指令执行超时"},
    {0x0C, "舱门打开指令执行超时"},
    {0x0D, "前后归正杆夹紧力矩超限"},
    {0x0E, "充电归正杆夹紧力矩超限"},
    {0x0F, "前后归正杆张开指令超时"},
    {0x10, "前后归正杆夹紧指令超时"},
    {0x11, "充电归正杆张开指令超时"},
    {0x12, "充电归正杆夹紧指令超时"},
    {0x17, "PLC不在就绪或待机，拒绝动作"},
    {0x18, "检测到有飞机，拒绝复位"},
    {0x19, "拨杆总故障"},
    {0x24, "步进电机异常"},
    {0x25, "步进电机异常"},
    {0x26, "步进电机异常"},
    {0x27, "步进电机异常"},
    {0x28, "步进电机异常"},
    {0x29, "步进电机异常"},
};

const CodeName kAlarms[] = {
    {0x01, "有轴未就绪，禁止整机复位"},
    {0x02, "手动状态，禁止整机复位"},
    {0x03, "指令执行中，拒绝新指令"},
    {0x04, "有飞机开门，但检测无飞机"},
    {0x05, "有飞机关门，但检测无飞机"},
    {0x06, "无飞机开门，但检测有飞机"},
    {0x07, "无飞机关门，但检测有飞机"},
    {0x08, "非拨杆流程中，拨杆B点无信号"},
    {0x09, "非拨杆流程中，拨杆A点有信号"},
};

QString lookup(const CodeName *table, int count, quint16 code)
{
    for (int i = 0; i < count; ++i) {
        if (table[i].code == code) {
            return QString::fromUtf8(table[i].name);
        }
    }
    return QString();
}

bool knownReply(quint8 status)
{
    return status == kStatusExecuting || status == kStatusRejected
           || status == kStatusOk || status == kStatusFail;
}

QString faultOrRaw(quint16 code, bool processCode)
{
    if (code == 0) {
        return QStringLiteral("无");
    }
    const QString named = processCode ? processFaultText(static_cast<quint8>(code))
                                      : driverFaultText(code);
    if (!named.isEmpty() && !named.startsWith(QStringLiteral("未登记"))) {
        return named;
    }
    return QStringLiteral("0x%1").arg(code, 4, 16, QLatin1Char('0'));
}

} // namespace

QString hexBytes(const QByteArray &frame)
{
    return QString::fromLatin1(frame.toHex(' ')).toUpper();
}

quint16 readBe16(const QByteArray &frame, int index)
{
    return (static_cast<quint8>(frame.at(index)) << 8)
           | static_cast<quint8>(frame.at(index + 1));
}

qint16 readBeS16(const QByteArray &frame, int index)
{
    return static_cast<qint16>(readBe16(frame, index));
}

int addressOf(quint8 cmd)
{
    switch (cmd) {
    case kCmdHealth: return kAddrHealth;
    case kCmdServo: return kAddrServo;
    case kCmdBattery: return kAddrBattery;
    case kCmdDoor: return kAddrDoor;
    case kCmdGripFr: return kAddrGripFr;
    case kCmdGripCh: return kAddrGripCh;
    case kCmdPlcCtrl: return kAddrPlcCtrl;
    case kCmdNetwork: return kAddrNetwork;
    default: return -1;
    }
}

QString commandName(quint8 cmd)
{
    switch (cmd) {
    case kCmdHealth: return QStringLiteral("健康状态");
    case kCmdServo: return QStringLiteral("伺服信息");
    case kCmdBattery: return QStringLiteral("电池状态");
    case kCmdDoor: return QStringLiteral("舱门");
    case kCmdGripFr: return QStringLiteral("前后归正");
    case kCmdGripCh: return QStringLiteral("充电归正");
    case kCmdPlcCtrl: return QStringLiteral("PLC控制");
    case kCmdNetwork: return QStringLiteral("网络状态");
    default: return QStringLiteral("未知命令0x%1").arg(cmd, 2, 16, QLatin1Char('0'));
    }
}

QString replyStatusText(quint8 status)
{
    switch (status) {
    case kStatusExecuting: return QStringLiteral("执行中");
    case kStatusRejected: return QStringLiteral("拒绝执行");
    case kStatusOk: return QStringLiteral("执行成功");
    case kStatusFail: return QStringLiteral("执行失败");
    case 0x00: return QStringLiteral("尚无回复");
    default: return QStringLiteral("未定义状态0x%1").arg(status, 2, 16, QLatin1Char('0'));
    }
}

QString plcStateText(quint8 state)
{
    switch (state) {
    case 1: return QStringLiteral("故障");
    case 2: return QStringLiteral("手动");
    case 3: return QStringLiteral("自动(待自检)");
    case 4: return QStringLiteral("就绪");
    case 5: return QStringLiteral("待机");
    default: return QStringLiteral("未知(%1)").arg(state);
    }
}

QString doorOpcodeText(quint8 opcode)
{
    switch (opcode) {
    case kDoorOpenWithPlane: return QStringLiteral("有飞机开门");
    case kDoorCloseWithPlane: return QStringLiteral("有飞机关门");
    case kDoorOpenNoPlane: return QStringLiteral("无飞机开门");
    case kDoorCloseNoPlane: return QStringLiteral("无飞机关门");
    default: return QStringLiteral("未定义操作码0x%1").arg(opcode, 2, 16, QLatin1Char('0'));
    }
}

QString gripOpcodeText(quint8 opcode)
{
    switch (opcode) {
    case kGripOpen: return QStringLiteral("张开");
    case kGripClose: return QStringLiteral("夹紧");
    default: return QStringLiteral("未定义操作码0x%1").arg(opcode, 2, 16, QLatin1Char('0'));
    }
}

QString controlItemName(int index)
{
    switch (index) {
    case kCtrlReset: return QStringLiteral("PLC复位");
    case kCtrlDeviceFan: return QStringLiteral("设备风扇");
    case kCtrlCabinetFan: return QStringLiteral("电控柜风扇");
    case kCtrlAirCondition: return QStringLiteral("设备空调");
    case kCtrlChargerPower: return QStringLiteral("充电器电源");
    case kCtrlQrLight: return QStringLiteral("二维码灯");
    default: return QStringLiteral("未知控制项");
    }
}

QString networkItemName(int index)
{
    switch (index) {
    case kNetMqtt: return QStringLiteral("MQTT");
    case kNetWindRain: return QStringLiteral("风速雨雪");
    case kNetTempHum: return QStringLiteral("温湿度");
    case kNetUps: return QStringLiteral("UPS");
    case kNetCharger: return QStringLiteral("充电器链路");
    default: return QStringLiteral("未知链路");
    }
}

QString driverFaultText(quint16 code)
{
    if (code == 0) {
        return QStringLiteral("无");
    }
    const QString named = lookup(kDriverFaults, int(sizeof(kDriverFaults) / sizeof(kDriverFaults[0])), code);
    if (!named.isEmpty()) {
        return named;
    }
    return QStringLiteral("未登记驱动器故障 0x%1").arg(code, 4, 16, QLatin1Char('0'));
}

QString processFaultText(quint8 code)
{
    if (code == 0) {
        return QStringLiteral("无");
    }
    const QString named = lookup(kProcessFaults, int(sizeof(kProcessFaults) / sizeof(kProcessFaults[0])), code);
    if (!named.isEmpty()) {
        return named;
    }
    return QStringLiteral("未登记流程故障 0x%1").arg(code, 2, 16, QLatin1Char('0'));
}

QString alarmText(quint8 code)
{
    if (code == 0) {
        return QStringLiteral("无");
    }
    const QString named = lookup(kAlarms, int(sizeof(kAlarms) / sizeof(kAlarms[0])), code);
    if (!named.isEmpty()) {
        return named;
    }
    return QStringLiteral("未登记告警 0x%1").arg(code, 2, 16, QLatin1Char('0'));
}

QByteArray blankFrame(quint8 cmd)
{
    QByteArray frame(kFrameBytes, 0);
    frame[0] = static_cast<char>(kDeviceType);
    frame[1] = static_cast<char>(cmd);
    frame[2] = 0x00;
    return frame;
}

QByteArray buildMotionFrame(quint8 cmd, quint8 opcode, quint8 planeModel)
{
    QByteArray frame = blankFrame(cmd);
    // 字节 4 = 下标 3，字节 6 = 下标 5。低字节按表保持 0。
    frame[3] = static_cast<char>(opcode);
    frame[5] = static_cast<char>(planeModel);
    return frame;
}

void setControlPair(QByteArray &frame, int itemIndex, quint8 opcode)
{
    const int index = 3 + itemIndex * 2;
    if (frame.size() < kFrameBytes || index + 1 >= kFrameBytes) {
        return;
    }
    frame[index] = static_cast<char>(opcode);
    frame[index + 1] = 0x00;
}

bool parseHealth(const QByteArray &frame, HealthStatus *out, QString *error)
{
    if (frame.size() < kFrameBytes || !out) {
        if (error) *error = QStringLiteral("健康状态帧长度不足");
        return false;
    }
    HealthStatus s;
    s.seq = static_cast<quint8>(frame.at(3));
    s.plcState = static_cast<quint8>(frame.at(4));
    s.paddleFault = static_cast<quint8>(frame.at(5));
    s.topDoorFault = readBe16(frame, 6);
    s.liftFault = readBe16(frame, 8);
    s.gripFrFault = readBe16(frame, 10);
    s.gripChFault = readBe16(frame, 12);
    s.alarmRaw = readBe16(frame, 16);
    s.plcFault = static_cast<quint8>(frame.at(18));
    s.mechanism = static_cast<quint8>(frame.at(19));
    s.sensorA = static_cast<quint8>(frame.at(24));
    s.sensorB = static_cast<quint8>(frame.at(25));
    s.inputs = static_cast<quint8>(frame.at(28));
    s.outputs = static_cast<quint8>(frame.at(31));

    s.doorOpen = s.mechanism & 0x80;
    s.doorClose = s.mechanism & 0x40;
    s.gripFrOpen = s.mechanism & 0x20;
    s.gripFrClose = s.mechanism & 0x10;
    s.gripChOpen = s.mechanism & 0x08;
    s.gripChClose = s.mechanism & 0x04;
    // 名称行：bit1 拨杆在 B 点，bit0 拨杆在 A 点。取值说明行把 A/B 写反了，这里跟名称行。
    s.paddleAtB = s.mechanism & 0x02;
    s.paddleAtA = s.mechanism & 0x01;

    s.smoke = s.inputs & 0x10;
    s.planePresent = s.inputs & 0x08;
    s.automatic = s.inputs & 0x04;
    s.estop = s.inputs & 0x02;

    s.qrLight = s.outputs & 0x20;
    s.chargerPower = s.outputs & 0x10;
    s.airCondition = s.outputs & 0x08;
    s.cabinetFan = s.outputs & 0x04;
    s.deviceFan = s.outputs & 0x02;
    s.resetDone = s.outputs & 0x01;
    *out = s;
    return true;
}

bool parseServo(const QByteArray &frame, ServoStatus *out, QString *error)
{
    if (frame.size() < kFrameBytes || !out) {
        if (error) *error = QStringLiteral("伺服帧长度不足");
        return false;
    }
    ServoStatus s;
    s.seq = static_cast<quint8>(frame.at(3));
    s.hour = static_cast<quint8>(frame.at(5));
    s.minute = static_cast<quint8>(frame.at(6));
    s.second = static_cast<quint8>(frame.at(7));
    // 表把坐标备注误写成“百分比力矩”。这里按有符号原值显示，不另乘系数。
    s.torqueTopDoor = readBeS16(frame, 8);
    s.torqueLift = readBeS16(frame, 10);
    s.torqueGripFr = readBeS16(frame, 12);
    s.torqueGripCh = readBeS16(frame, 14);
    s.posTopDoor = readBeS16(frame, 16);
    s.posLift = readBeS16(frame, 18);
    s.posGripFr = readBeS16(frame, 20);
    s.posGripCh = readBeS16(frame, 22);
    *out = s;
    return true;
}

CommandReply parseCommandReply(const QByteArray &frame)
{
    CommandReply reply;
    if (frame.size() < kFrameBytes) {
        reply.summary = QStringLiteral("回复帧长度不足");
        return reply;
    }
    const quint8 atByte28 = static_cast<quint8>(frame.at(27));
    const quint8 atByte3 = static_cast<quint8>(frame.at(2));
    if (knownReply(atByte28)) {
        reply.recognized = true;
        reply.statusAtByte28 = true;
        reply.status = atByte28;
        reply.alarm = static_cast<quint8>(frame.at(29));
        reply.fault = static_cast<quint8>(frame.at(31));
    } else if (knownReply(atByte3)) {
        reply.recognized = true;
        reply.statusAtByte28 = false;
        reply.status = atByte3;
        reply.alarm = static_cast<quint8>(frame.at(29));
        reply.fault = static_cast<quint8>(frame.at(31));
    } else {
        reply.summary = QStringLiteral("字节28=%1，字节3=%2，都不是回复状态")
                            .arg(replyStatusText(atByte28), replyStatusText(atByte3));
        return reply;
    }

    QStringList parts;
    parts << QStringLiteral("回复位置=%1").arg(reply.statusAtByte28
                                              ? QStringLiteral("字节28(接口表)")
                                              : QStringLiteral("字节3(流程表)，与接口表不一致"));
    parts << replyStatusText(reply.status);
    if (reply.status == kStatusRejected) {
        parts << QStringLiteral("告警=%1").arg(alarmText(reply.alarm));
    }
    if (reply.status == kStatusFail) {
        parts << QStringLiteral("故障=%1").arg(processFaultText(reply.fault));
    }
    reply.summary = parts.join(QStringLiteral("，"));
    return reply;
}

QString describeHealth(const HealthStatus &s)
{
    const QString alarmName = (s.alarmRaw <= 0xFF) ? alarmText(static_cast<quint8>(s.alarmRaw))
                                                   : QStringLiteral("0x%1").arg(s.alarmRaw, 4, 16, QLatin1Char('0'));
    return QStringLiteral("序号=%1 状态=%2 顶门开=%3 顶门关=%4 前后张=%5 前后夹=%6 充电张=%7 充电夹=%8 "
                          "拨杆A=%9 拨杆B=%10 急停=%11 烟雾=%12 飞机=%13 手自动=%14 "
                          "告警=%15 PLC故障=%16 顶门轴=%17 升降轴=%18 前后轴=%19 充电轴=%20 拨桨叶=%21")
        .arg(int(s.seq))
        .arg(plcStateText(s.plcState))
        .arg(int(s.doorOpen)).arg(int(s.doorClose))
        .arg(int(s.gripFrOpen)).arg(int(s.gripFrClose))
        .arg(int(s.gripChOpen)).arg(int(s.gripChClose))
        .arg(int(s.paddleAtA)).arg(int(s.paddleAtB))
        .arg(int(s.estop)).arg(int(s.smoke)).arg(int(s.planePresent))
        .arg(s.automatic ? QStringLiteral("自动") : QStringLiteral("手动"))
        .arg(alarmName)
        .arg(processFaultText(s.plcFault))
        .arg(faultOrRaw(s.topDoorFault, false))
        .arg(faultOrRaw(s.liftFault, false))
        .arg(faultOrRaw(s.gripFrFault, false))
        .arg(faultOrRaw(s.gripChFault, false))
        .arg(s.paddleFault == 0 ? QStringLiteral("无")
                                : driverFaultText(s.paddleFault));
}

QString describeServo(const ServoStatus &s)
{
    return QStringLiteral("序号=%1 时间=%2:%3:%4 力矩(顶门/升降/前后/充电)=%5/%6/%7/%8 "
                          "坐标=%9/%10/%11/%12")
        .arg(int(s.seq))
        .arg(int(s.hour), 2, 10, QLatin1Char('0'))
        .arg(int(s.minute), 2, 10, QLatin1Char('0'))
        .arg(int(s.second), 2, 10, QLatin1Char('0'))
        .arg(s.torqueTopDoor).arg(s.torqueLift).arg(s.torqueGripFr).arg(s.torqueGripCh)
        .arg(s.posTopDoor).arg(s.posLift).arg(s.posGripFr).arg(s.posGripCh);
}

QString describeTxFrame(quint8 cmd, const QByteArray &frame)
{
    if (frame.size() < kFrameBytes) {
        return QStringLiteral("帧长度错误");
    }
    QString detail;
    if (cmd == kCmdDoor) {
        detail = QStringLiteral("操作码=%1 机型=0x%2")
                     .arg(doorOpcodeText(static_cast<quint8>(frame.at(3))))
                     .arg(static_cast<quint8>(frame.at(5)), 2, 16, QLatin1Char('0'));
    } else if (cmd == kCmdGripFr || cmd == kCmdGripCh) {
        detail = QStringLiteral("操作码=%1 机型=0x%2")
                     .arg(gripOpcodeText(static_cast<quint8>(frame.at(3))))
                     .arg(static_cast<quint8>(frame.at(5)), 2, 16, QLatin1Char('0'));
    } else if (cmd == kCmdPlcCtrl || cmd == kCmdNetwork) {
        QStringList items;
        const int count = (cmd == kCmdPlcCtrl) ? kCtrlCount : kNetCount;
        for (int i = 0; i < count; ++i) {
            const quint8 value = static_cast<quint8>(frame.at(3 + i * 2));
            const QString name = (cmd == kCmdPlcCtrl) ? controlItemName(i) : networkItemName(i);
            items << QStringLiteral("%1=0x%2").arg(name).arg(value, 2, 16, QLatin1Char('0'));
        }
        detail = items.join(QStringLiteral(" "));
    }
    return QStringLiteral("%1 %2 帧=%3").arg(commandName(cmd), detail, hexBytes(frame));
}

bool selfCheck(QString *error)
{
    const QByteArray door = buildMotionFrame(kCmdDoor, kDoorOpenWithPlane, kPlaneModelQ20);
    if (door.size() != kFrameBytes || static_cast<quint8>(door.at(0)) != kDeviceType
        || static_cast<quint8>(door.at(1)) != kCmdDoor
        || static_cast<quint8>(door.at(3)) != kDoorOpenWithPlane
        || static_cast<quint8>(door.at(4)) != 0
        || static_cast<quint8>(door.at(5)) != kPlaneModelQ20) {
        if (error) *error = QStringLiteral("舱门帧字节位置与接口表不一致");
        return false;
    }
    QByteArray ctrl = blankFrame(kCmdPlcCtrl);
    setControlPair(ctrl, kCtrlAirCondition, kOpOn);
    if (static_cast<quint8>(ctrl.at(9)) != kOpOn || static_cast<quint8>(ctrl.at(10)) != 0) {
        if (error) *error = QStringLiteral("空调控制没有落在字节10");
        return false;
    }
    QByteArray net = blankFrame(kCmdNetwork);
    setControlPair(net, kNetMqtt, kLinkDown);
    if (static_cast<quint8>(net.at(3)) != kLinkDown) {
        if (error) *error = QStringLiteral("MQTT 状态没有落在字节4");
        return false;
    }
    QByteArray sample(kFrameBytes, 0);
    sample[1] = static_cast<char>(kCmdDoor);
    sample[27] = static_cast<char>(kStatusFail);
    sample[31] = static_cast<char>(0x0B);
    const CommandReply reply = parseCommandReply(sample);
    if (!reply.recognized || !reply.statusAtByte28 || reply.fault != 0x0B) {
        if (error) *error = QStringLiteral("回复状态没有从字节28读取");
        return false;
    }
    return true;
}

} // namespace Protocol
