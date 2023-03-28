//
// Created by 58226 on 2023/2/6.
//

#ifndef TIMELINEDEFINATION_H
#define TIMELINEDEFINATION_H
#include <QColor>
#include <QDebug>
#include "extensionmethods.h"

///时间线刻度区域占总绘制区高度（百分比）
const int PERCENT_OF_TICK = 50;

///最小的宽度像素
const int MIN_TICK_WIDTH = 20;

///控件间距
const int WIDGET_MARGIN = 9;

///顶部间距
const int TOP_MARGIN = 10;

///默认宽
const int DEFAULT_WIDGET_WIDTH = 1920;

///默认高
const int DEFAULT_WIDGET_HEIGHT = 1080;

///每秒帧
const int FRAME_PER_SECOND = 25;

///时间线刻度字体大小
const int TIME_TICK_FONT_SIZE = 12;

///时间线缩放值字体大小
const int SCALE_CODE_FONT_SIZE = 20;

///轨道高度
const int TRACK_HEIGHT = 30;

///自动滚动的单次距离（以及触发自动滚动的宽度）
const int SCROLL_DISTANCE = 10;

///时间线锚点宽度
const int TIMELINE_ANCHOR_WIDTH = 20;

///锚点线触发宽度
const int TIMELINE_ANCHOR_BODY_WIDTH = 4;

///时间线滚动变更值
const int WHEEL_SCALE_DELTA = 2;

///切片触发扩张变更值
const int CLIP_SPAN_UNIT = 2;

///触发拖拽变长的阈值
const int CLIP_DRAG_EXPAND_LIMIT = 2;

///时间线绘制颜色
const QColor DRAW_COLOR = QColor(160, 160, 160);

///背景深色
const QColor BACK_DEEP_COLOR = QColor(40, 40, 46);

///背景浅色
const QColor BACK_LIGHT_COLOR = QColor(45, 45, 53);

///锚点绘制色
const QColor ANCHOR_COLOR = QColor(230, 75, 61);

///黑笔
const QColor BLACK_COLOR = QColor(0, 0, 0);

///效果切片色
const QColor EFFECT_CLIP_COLOR = QColor(157, 171, 194);

///字幕切片色
const QColor SUBTITLE_CLIP_COLOR = QColor(2, 143, 94);

///贴纸切片色
const QColor STICKER_CLIP_COLOR = QColor(86, 105, 243);

///类别
enum SpecificType
{
    //无
    None = 0,
    //特技
    Effect,
    //贴纸
    Sticker,
    //字幕
    Subtitle,
    //图表
    PictureForm,
    //蒙版
    Mask,
};

#pragma region function
///用于Ruler上的数值转换
static QString FrameToTimeCode(ulong curFrame, int FrameRate)
{
    int sec = (int)(curFrame / FrameRate);
    int h = sec / 60 / 60;
    int m = (sec / 60) % 60;
    int s = sec % 60;
    int f = (int)(curFrame - FrameRate * sec);
    return QString("%1:%2:%3-%4").arg(h, 2, 10, QLatin1Char('0')).arg(m, 2, 10, QLatin1Char('0'))
        .arg(s, 2, 10, QLatin1Char('0')).arg(f, 2, 10, QLatin1Char('0'));
}
///用于Tick上的数值转换
static QString FrameToScaleCode(ulong curFrameTick, int FrameRate)
{

    if (curFrameTick < FrameRate) {

        return std::move(QString::asprintf("%ld f", curFrameTick));
    }
    else if (curFrameTick >= FrameRate && curFrameTick < 60 * FrameRate) {
        return std::move(QString::asprintf("%.1f s", (float)curFrameTick / (float)FrameRate));
    }
    else if (curFrameTick >= 60 * FrameRate && curFrameTick < 60 * 60 * FrameRate) {
        return std::move(QString::asprintf("%.1f m", (float)curFrameTick / 60.0 / (float)FrameRate));
    }
    else if (curFrameTick >= 60 * 60 * FrameRate) {
        return std::move(QString::asprintf("%.1f h", curFrameTick / 60.0 / 60.0 / (float)FrameRate));
    }
    return "";
}
#pragma  endregion

#endif //TIMELINEDEFINATION_H
