//
// Created by 58226 on 2023/2/9.
//

#include "clip/clipitem.h"
#include "timelinedefination.h"
#include "timelinewidget.h"
#include "intervalwatcher.h"
#include <QGraphicsSceneMouseEvent>
#include <QApplication>
#define timeline() (GET_POINTER<TimelineWidget>())
ClipItem::ClipItem(QGraphicsItem* parent)
	: QGraphicsItem(parent)
{

	init();

}

ClipItem::ClipItem(const QString& key, QGraphicsItem* parent): QGraphicsItem(parent)
{
	m_mimeKey = key;
	init();
}
ClipItem::~ClipItem()
{
	removeShadow();
	if (m_leftHandle->isAddToScene()) {
		m_leftHandle->setAddToScene(false);
		scene()->removeItem(m_leftHandle);
	}
	if (m_rightHandle->isAddToScene()) {
		m_rightHandle->setAddToScene(false);
		scene()->removeItem(m_rightHandle);
	}
	qDebug()<<m_mimeKey<<" start release";
	SAFE_DELETE(m_shadow);
	SAFE_DELETE(m_leftHandle);
	SAFE_DELETE(m_rightHandle);
	qDebug()<<m_mimeKey<<" released";
}
void ClipItem::init()
{
	setFlags(ItemIsSelectable | ItemSendsGeometryChanges);
	setAcceptHoverEvents(true);
	setAcceptedMouseButtons(Qt::LeftButton);
	m_shadow = new ShadowClipItem(m_mimeKey, this);
	m_leftHandle = new ClipDragHandle(this);
	m_rightHandle = new ClipDragHandle(this);
	//for test
	m_image.load("./test.png");
}
void ClipItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	//intervalwatcher iw;
	//iw.start();
	auto countOfTrack = timeline()->getTrackCount();
	auto trackBodyHeight = countOfTrack * TRACK_HEIGHT;
	auto trackData = getTrackData(m_trackMimeKey);
	if (trackData.isDefaultData())
		return;
	auto curData = trackData.getClip(m_mimeKey);
	auto xIndex = ceil(((double)(curData.startPos) / timeline()->frameTick() * MIN_TICK_WIDTH));
	auto width = ceil((double)(curData.duration) / timeline()->frameTick() * MIN_TICK_WIDTH);
	auto curArea = timeline()->getArea(TimelineWidget::RightBottom);
	double yIndex = trackData.index * TRACK_HEIGHT + 1;
	bool shouldDrawText = false;
	bool shouldDrawPic = false;
	if (trackBodyHeight < curArea.height())//y center alignment
	{
		yIndex += (curArea.height() - trackBodyHeight) / 2.0;
	}
	QColor paintColor = Qt::red;
	switch (curData.type) {
	case SpecificType::Subtitle:
		shouldDrawText = true;
		paintColor = SUBTITLE_CLIP_COLOR;
		break;
	case SpecificType::Effect:
		paintColor = EFFECT_CLIP_COLOR;
		shouldDrawPic = true;
		shouldDrawText = true;
		break;
	case SpecificType::Sticker:
		shouldDrawPic = true;
		paintColor = STICKER_CLIP_COLOR;
		break;
	default:
		break;
	}
	painter->setRenderHint(QPainter::Antialiasing);
	painter->setPen(QPen(Qt::white));
	painter->setBrush(QBrush(paintColor));
	QPainterPath path;
	path.addRoundedRect(QRectF(xIndex, yIndex + 0.5, width, TRACK_HEIGHT - 2), 5, 5);
	painter->fillPath(path, QBrush(paintColor));
	auto limitedArea = timeline()->getViewPort(TimelineWidget::RightBottom);
	if (m_isOnHover || timeline()->isSelected(m_mimeKey)) {
		painter->drawRoundedRect(QRectF(xIndex, yIndex, width, TRACK_HEIGHT - 1), 5, 5);
	}

	//TODO:draw extra info of clip,maybe we should change the mime data struct
	int startX = xIndex < limitedArea.x() ? limitedArea.x() : xIndex;
	//int endX = (xIndex + width) > limitedArea.right() ? limitedArea.right() : (xIndex + width);
	//for test
	painter->setPen(QPen(Qt::white));
	for (int curX = startX; curX <= xIndex + width; curX += 120) {
		if ((xIndex + width) - curX < 120)
			return;
		if (shouldDrawText) {
			painter->drawText(curX, (int)yIndex, 120, TRACK_HEIGHT, Qt::AlignHCenter | Qt::AlignVCenter, m_mimeKey);
			//painter->drawText(curX, (int)yIndex + 10, 20, TRACK_HEIGHT, Qt::AlignHCenter | Qt::AlignVCenter, "测试");
			//painter->drawText(curX, (int)yIndex + 20, 20, TRACK_HEIGHT, Qt::AlignHCenter | Qt::AlignVCenter, "测试");
		}
		if (shouldDrawPic) {
			painter->drawPixmap(QRect(curX, yIndex, 20, 20), m_image);
		}
	}
	//iw.stop();
	//qDebug()<<iw.milliSecond()<<" ms to finish clip draw";

}
QRectF ClipItem::boundingRect() const
{
	auto trackBodyHeight = timeline()->getTrackCount() * TRACK_HEIGHT;
	auto trackData = getTrackData(m_trackMimeKey);
	auto curData = trackData.getClip(m_mimeKey);
	auto xIndex = ((double)(curData.startPos) / timeline()->frameTick()) * MIN_TICK_WIDTH;
	auto width = ((double)(curData.duration) / timeline()->frameTick()) * MIN_TICK_WIDTH;
	auto curArea = timeline()->getArea(TimelineWidget::RightBottom);
	double yIndex = trackData.index * TRACK_HEIGHT;
	if (trackBodyHeight < curArea.height())//y center alignment
	{
		yIndex += (curArea.height() - trackBodyHeight) / 2.0;
	}
	return { xIndex, yIndex, width, TRACK_HEIGHT };
}
bool ClipItem::insertToTrack(const QString& trackKey)
{
	if (!m_trackMimeKey.isEmpty())
		return false;
	m_trackMimeKey = trackKey;
	return true;
}
bool ClipItem::removeFromTrack()
{
	removeShadow();
	if (m_leftHandle->isAddToScene()) {
		m_leftHandle->setAddToScene(false);
		this->scene()->removeItem(m_leftHandle);
	}
	if (m_rightHandle->isAddToScene()) {
		m_rightHandle->setAddToScene(false);
		this->scene()->removeItem(m_rightHandle);
	}
	if (m_trackMimeKey.isEmpty()) {
		return false;
	}
	m_trackMimeKey = "";
	return true;
}
ClipMime ClipItem::getMimeData(const QString& clipKey, bool searchWhenTrackKeyEmpty) const
{
	return getTrackData(m_trackMimeKey, searchWhenTrackKeyEmpty).getClip(clipKey);
}
TrackMime ClipItem::getTrackData(const QString& trackKey, bool searchWhenTrackKeyEmpty) const
{
	if (trackKey.isEmpty()) {
		if (!searchWhenTrackKeyEmpty)
			return {};
		auto TrackCount = timeline()->getTrackCount();
		TrackMime curData;
		for (int i = 0; i < TrackCount; i++) {
			curData = timeline()->getTrackData(i);
			if (!curData.getClip(m_mimeKey).isDefaultData()) {
				break;
			}
		}
		return curData;
	}
	else {
		TrackMime curTrackData;
		if (!timeline()->getTrackData(curTrackData, trackKey))
			return {};
		return curTrackData;
	}

}
QVariant ClipItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value)
{
	return QGraphicsItem::itemChange(change, value);
}
void ClipItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	m_prePoint = event->scenePos();
	m_originPoint = event->scenePos();
	if (event->button() == Qt::MouseButton::LeftButton) {
		if (QApplication::keyboardModifiers().testFlag(Qt::ControlModifier)) {
			timeline()->setSelectedClip(QList<QString>{m_mimeKey}, timeline()->isSelected(m_mimeKey));
		}
		else {

			if (!timeline()->isSelected(m_mimeKey)) {
				timeline()->setSelectedClip(m_mimeKey, false);
			}
			if (!m_isMouseDrag) {
				if (boundingRect().width() > CLIP_DRAG_EXPAND_LIMIT * 2 &&
					(m_prePoint.x() >= boundingRect().x()) &&
					abs(m_prePoint.x() - boundingRect().x()) <= CLIP_DRAG_EXPAND_LIMIT) {
					m_isLeftExpand = true;
					if (!this->scene()->items().contains(m_shadow))
						this->scene()->addItem(m_shadow);
				}

				else if (boundingRect().width() > CLIP_DRAG_EXPAND_LIMIT * 2 &&
					(m_prePoint.x()) <= (boundingRect().right()) &&
					abs(boundingRect().right() - m_prePoint.x()) <= CLIP_DRAG_EXPAND_LIMIT) {

					m_isRightExpand = true;
					if (!this->scene()->items().contains(m_shadow))
						this->scene()->addItem(m_shadow);
				}
				else {
					timeline()->clipMoved(0, 0, false, m_originPoint.x() == m_prePoint.x());
					m_isMouseDrag = true;
				}
			}

		}
		event->accept();
	}
	else {
		QGraphicsItem::mousePressEvent(event);
	}

}
void ClipItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
	auto curPos = event->scenePos();
	auto curMime = getMimeData(m_mimeKey, true);
	//drag move
	if (m_isMouseDrag) {
		event->accept();
		m_isMouseDrag = false;

		timeline()->clipMoved(0, 0, true, m_originPoint.x() == m_prePoint.x());
	}
	//left expand
	else if (m_isLeftExpand) {
		removeShadow();
		double deltaX = timeline()->percentPerUnit() * (m_shadowRect.x() - boundingRect().x());
		auto curS = (int64_t)curMime.startPos + (int64_t)(deltaX);
		auto curD = (int64_t)curMime.duration - (int64_t)(deltaX);
		curMime.startPos = curS <= 0 ? 0 : curS;
		curMime.duration = curD <= 0 ? 1 : curD;
		//alter datas;
		timeline()->alterClipData(curMime.id, curMime.trackId, curMime);
		event->accept();
		m_isLeftExpand = false;
		checkExpandHandle(boundingRect());
	}
	//right expand
	else if (m_isRightExpand) {
		removeShadow();
		double deltaX = timeline()->percentPerUnit() * ((m_shadowRect.x() + m_shadowRect.width())
			- (boundingRect().width() + boundingRect().x()));
		auto curD = (int64_t)curMime.duration + (int64_t)(deltaX);
		curMime.duration = curD < 0 ? 1 : curD;
		timeline()->alterClipData(curMime.id, curMime.trackId, curMime);
		event->accept();
		m_isRightExpand = false;
		checkExpandHandle(boundingRect());
	}
	else {
		QGraphicsItem::mouseReleaseEvent(event);
	}

}
void ClipItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
	auto xDelta = event->scenePos().x() - m_prePoint.x();
	auto yDelta = event->scenePos().y() - m_prePoint.y();
	if (m_isMouseDrag) {

		m_prePoint = event->scenePos();
		if (xDelta != 0 || yDelta != 0)
		{
			timeline()->clipMoved(xDelta, yDelta, false, false);
		}
		event->accept();
	}
	else if (m_isLeftExpand || m_isRightExpand) {

		if (m_isLeftExpand) {
			m_shadowRect = { boundingRect().x() + xDelta, boundingRect().y(), boundingRect().width() - xDelta,
							boundingRect().height() };
		}
		else {
			m_shadowRect =
			{ boundingRect().x(), boundingRect().y(), boundingRect().width() + xDelta, boundingRect().height() };
		}
		m_shadow->setDrawRect(m_shadowRect);
		m_shadow->forceUpdate();
		event->accept();
	}
	else {
		QGraphicsItem::mouseMoveEvent(event);
	}

}
void ClipItem::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
	m_isOnHover = true;
	checkExpandHandle(boundingRect());
	QGraphicsItem::hoverEnterEvent(event);
}
void ClipItem::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
	m_isOnHover = false;
	if (scene() != nullptr) {
		auto items = scene()->items(event->scenePos());
		bool isAtItem = false;
		for (auto cur : items) {
			if (cur == this) {
				isAtItem = true;
				break;
			}
		}
		if (!isAtItem) {
			if (m_leftHandle->isAddToScene()) {
				m_leftHandle->setAddToScene(false);
				scene()->removeItem(m_leftHandle);
			}
			if (m_rightHandle->isAddToScene()) {
				m_rightHandle->setAddToScene(false);
				scene()->removeItem(m_rightHandle);
			}

		}
		prepareGeometryChange();
		scene()->update(timeline()->getViewPort(TimelineWidget::RightBottom));
	}

	QGraphicsItem::hoverLeaveEvent(event);
}
double ClipItem::getStartPosAfterDragMove(ClipMime& curMime, bool shouldIgnore)
{
	//auto xDelta = m_shadowRect.x() - m_originRect.x();
	if (curMime.isDefaultData()) {
		curMime = getMimeData(m_mimeKey);
	}
	if (curMime.isDefaultData())
		return 0;
	if (shouldIgnore)
		return curMime.startPos;
	//auto curFrameMoved = (long)ceil(xDelta * timeline()->percentPerUnit());
	auto curX = (mapFromScene(m_shadowRect).boundingRect().x() * timeline()->percentPerUnit());//curMime.startPos;
	//qDebug()<<"with "<<curX<<":["<<mapFromScene(m_shadowRect).boundingRect().x()<<","<<timeline()->percentPerUnit()<<"]";
	return curX;
}
void ClipItem::clipDrag(double x, double y)
{
	if (this->scene() == nullptr)
		return;
	if (!m_isDragMoved) {
		m_isDragMoved = true;
		m_originRect = m_shadowRect = boundingRect();
		if (!this->scene()->items().contains(m_shadow))
			this->scene()->addItem(m_shadow);
	}
	else {
		m_shadowRect = QRectF(m_shadowRect.x() + x, m_shadowRect.y() + y,
			m_shadowRect.width(), m_shadowRect.height());
		if (m_shadow) {
			m_shadow->setDrawRect(m_shadowRect);
			m_shadow->forceUpdate();
		}
	}
}
ClipMime ClipItem::stopClipDrag(bool& isSceneNull, bool posNotMove, bool isMultiMode)
{
	if (this->scene() == nullptr)
	{
		isSceneNull = true;
		return {};
	}
	auto curMime = getMimeData(m_mimeKey);
	if (m_isDragMoved) {

		auto curPos = getStartPosAfterDragMove(curMime, posNotMove);
		if (curPos < 0)
		{
			//qDebug()<<"adjust with:"<<(abs(curPos)/timeline()->percentPerUnit());
			timeline()->alterClipMovement((abs(curPos) / timeline()->percentPerUnit()), 0);
			return {};
		}
		//qDebug()<<"origin with "<<curMime.startPos<<"after is:"<<curPos;
		curMime.startPos = (ulong)(curPos < 0 ? 0 : curPos);
		if (isMultiMode) {
			removeShadow();
			timeline()->alterClipData(curMime.id, m_trackMimeKey, curMime);//change X pos only, track pos changed outside;
			if (!m_isRemoved) {
				forceUpdate();
				checkExpandHandle(boundingRect());
			}

			return curMime;
		}
		TrackMime readyTrackData;
		// using center position to determine which track to insert
		bool isChangeTrack = false;
		auto originTrackKey = curMime.trackId;
		auto isMultiSelect = timeline()->m_selectedClips.count() > 1;
		auto changeTrackOpt = [&]() -> void
		{
			timeline()->getTrackByVerticalPos((mapFromScene(m_shadowRect).boundingRect().y() + TRACK_HEIGHT / 2.0),
				readyTrackData);
			if (!readyTrackData.isDefaultData()) {
				isChangeTrack = true;
				curMime.trackId = readyTrackData.id;
			}
		};

		changeTrackOpt();
		auto res = checkForCollision(curMime, originTrackKey);
		if (!res)//no collision
		{
			changeTrackOpt();
		}
		removeShadow();
		auto originKey = m_trackMimeKey;
		timeline()->alterClipData(curMime.id, originKey, curMime);
		if (originKey == curMime.trackId) {
			forceUpdate();
			checkExpandHandle(boundingRect());
		}
		m_isDragMoved = false;
	}
	return curMime;
}
void ClipItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
	timeline()->setSelectedClip(m_mimeKey, timeline()->isSelected(m_mimeKey));
	QGraphicsItem::mouseDoubleClickEvent(event);
}
void ClipItem::forceUpdate()
{

	if (m_isRemoved)
		return;
	if(m_shadow)
	{		
		m_shadow->forceUpdate();
	}
	if(m_leftHandle)
	{
		m_leftHandle->forceUpdate();
	}
	if(m_rightHandle)
	{
		m_rightHandle->forceUpdate();
	}
	prepareGeometryChange();
	update();
	//checkExpandHandle(boundingRect());
}
bool ClipItem::checkForCollision(ClipMime& curMime, const QString& originTrackKey)
{
	QList<TrackMime> curTracksData(timeline()->m_timelineData.tracks.begin(),
		timeline()->m_timelineData.tracks.end());
	auto curRange = ClipRange(curTracksData);
	curRange.oneClipChanged(curMime, originTrackKey);
	QList<QString> collisionItems;
	//	auto currentCollisionItems = ExtensionMethods::SourcesExtension<QGraphicsItem*>
	//		::where(m_shadow->collidingItems(),
	//			[&](QGraphicsItem* curItem) -> bool
	//			{
	//				auto clipItem = dynamic_cast<ClipItem*>(curItem);
	//				if (clipItem == nullptr)
	//					return false;
	//				return !timeline()->isSelected(
	//					clipItem->m_mimeKey) && clipItem->m_trackMimeKey == curMime.trackId;
	//			});
	auto hasCollision = curRange.hasCollision(curMime.trackId, curMime.id, collisionItems);
	//currentCollisionItems.count() > 0;//
	if (!hasCollision)
		return false;
	TrackMime trackData;
	if (!timeline()->getTrackData(trackData, originTrackKey.isEmpty() ? m_trackMimeKey : curMime.trackId)) {
		return false;
	}
	auto collisionClips = trackData.getClips([](const ClipMime& curItem) -> bool
		{ return !(timeline()->isSelected(curItem.id)); });
	QList<ClipMime> headClips, tailClips;
	ExtensionMethods::SourcesExtension<ClipMime>::classification(collisionClips,
		[&curMime](const ClipMime& item) -> bool
		{
			return item.startPos <= curMime.startPos;
		},
		headClips,
			tailClips);
	//let these clips in order
	std::sort(headClips.begin(), headClips.end(), [](const ClipMime& l, const ClipMime& r) -> bool
		{
			return l.startPos < r.startPos;
		});
	std::sort(tailClips.begin(), tailClips.end(), [](const ClipMime& l, const ClipMime& r) -> bool
		{
			return l.startPos < r.startPos;
		});


	ulong diffDelta = 0;
	auto selectedClips = timeline()->getAllSelectedClip();

	/// must has collision
	if (selectedClips.count() > 1)//more than two clips
	{
		return true;//do nothing
	}
	else//single clip
	{
		//for head ,depart it,get the diffDelta with last clip.
		auto collisionItem = ExtensionMethods::SourcesExtension<ClipMime>::
			lastOf(headClips,
				[&collisionItems](const ClipMime& curClip) -> bool
				{
					return ExtensionMethods::SourcesExtension<QString>
						::firstOf(collisionItems,
							[&](const QString& curItem) -> bool
							{
								//								auto clipItem = dynamic_cast<ClipItem*>(curItem);
								//								if (clipItem == nullptr)
								//									return false;
								return curItem == curClip.id;
							}, nullptr) != nullptr;
				},
				ClipMime());
		//auto clipItem = dynamic_cast<ClipItem *>(currentCollisionItems.last());
		//if (clipItem == nullptr)
		//return false;
		//auto collisionItem = clipItem->getMimeData(clipItem->m_mimeKey);
		ClipMime left, right;
		if (!collisionItem.isDefaultData()) {
			if (collisionItem.cutUp(curMime.startPos, left, right)) {
				right.startPos = left.startPos + left.duration + curMime.duration;
				diffDelta = right.duration;
				timeline()->removeClip(collisionItem);
				timeline()->addClip(curMime.trackId, left, true, false);
				timeline()->addClip(curMime.trackId, right, true, false);
			}
		}

		//for tail ,if front space is not enough,move tail clips.
		if (tailClips.count() > 0 && tailClips[0].startPos < curMime.endPosition()) {
			for (int i = 0; i < tailClips.count(); i++) {

				tailClips[i].startPos += ((curMime.endPosition() - tailClips[0].startPos) + diffDelta);
				timeline()->alterClipData(tailClips[i].id, tailClips[i].trackId, tailClips[i]);
			}

		}
		timeline()->updateMaxDuration();
		return true;
	}
}
void ClipItem::checkExpandHandle(const QRectF& clipRect)
{
	//return;
	//check for expand handle
	if (m_trackMimeKey.isEmpty() )
		return;
	if (clipRect.width() >= CLIP_DRAG_EXPAND_LIMIT * 2) {
		if (!m_leftHandle->isAddToScene()) {
			m_leftHandle->setAddToScene(true);
			this->scene()->addItem(m_leftHandle);
		}
		if (!m_rightHandle->isAddToScene()) {
			m_rightHandle->setAddToScene(true);
			this->scene()->addItem(m_rightHandle);
		}
		m_leftHandle->setDrawingRect(QRectF(clipRect.x(), clipRect.y(), CLIP_DRAG_EXPAND_LIMIT, TRACK_HEIGHT));
		m_rightHandle->setDrawingRect(QRectF(clipRect.x() + clipRect.width() - CLIP_DRAG_EXPAND_LIMIT,
			clipRect.y(),
			CLIP_DRAG_EXPAND_LIMIT,
			TRACK_HEIGHT));
	}
	else {
		if (m_leftHandle->isAddToScene()) {
			m_leftHandle->setAddToScene(false);
			scene()->removeItem(m_leftHandle);
		}
		if (m_rightHandle->isAddToScene()) {
			m_rightHandle->setAddToScene(false);
			scene()->removeItem(m_rightHandle);
		}
	}
	if (m_leftHandle) {
		m_leftHandle->forceUpdate();
	}
	if (m_rightHandle) {
		m_rightHandle->forceUpdate();
	}
}
bool ClipItem::preCheckForCollision(ClipMime& mime, TrackMime& targetTrack)
{
	auto curMime = getMimeData(m_mimeKey);
	auto curPos = getStartPosAfterDragMove(curMime, m_prePoint.x() == m_originPoint.x());
	curMime.startPos = curPos < 0 ? 0 : curPos;
	bool isChangeTrack = false;
	auto originTrackKey = curMime.trackId;
	//	auto currentCollisionItems = ExtensionMethods::SourcesExtension<QGraphicsItem*>
	//		::where(m_shadow->collidingItems(),
	//			[&](QGraphicsItem* curItem) -> bool
	//			{
	//				auto clipItem =
	//					dynamic_cast<ClipItem*>(curItem);
	//				if (clipItem == nullptr)
	//					return false;
	//				return !timeline()
	//					->isSelected(
	//						clipItem
	//						->m_mimeKey);
	//			});

	timeline()->getTrackByVerticalPos(mapFromScene(m_shadowRect).boundingRect().y() + TRACK_HEIGHT / 2.0, targetTrack);
	if (!targetTrack.isDefaultData()) {
		//isChangeTrack = true;
		curMime.trackId = targetTrack.id;
	}
	QList<TrackMime> curTracksData(timeline()->m_timelineData.tracks.begin(),
		timeline()->m_timelineData.tracks.end());

	auto curRange = ClipRange(curTracksData);
	curRange.oneClipChanged(curMime, originTrackKey);
	QList<QString> collisionItems;
	mime = curMime;
	curRange.hasCollision(curMime.trackId, curMime.id, collisionItems);
	bool res = collisionItems.count() > 0;//currentCollisionItems.count() > 0;
	if (collisionItems.count() > 0) {
		res = std::any_of(collisionItems.begin(), collisionItems.end(), [&](const QString& str) -> bool
			{
				return !timeline()->isSelected(str);
			});
	}
	if (res) {
		//qDebug() << "current with collision";
	}
	return res;
}
void ClipItem::removeShadow()
{

	if (m_shadow) {
		m_shadow->setDrawRect({ 0, 0, 0, 0 });
		m_shadow->forceUpdate();
	}
	m_shadowRect = boundingRect();
	if (scene() != nullptr)
	{
		if (scene()->items().contains(m_shadow))
			scene()->removeItem(m_shadow);
	}

}



