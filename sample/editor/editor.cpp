#include <editor.hpp>

EditScene::EditScene(void)
{
	selected_object_type = -1;
	selected_editable_object_type = -1;
	setSceneRect(0, 0, 600, 600);
	GamTexture t("sky.jpg");
	QPixmap p = t.pixmap();
	QBrush b(p);
	setBackgroundBrush(b);
	rect_object = new RectObject(10, 10, 30, 30);
	rect_object->setBrush(QColor("#00eeee"));
	connect(rect_object, SIGNAL(selectedSignal(int)),
			this, SLOT(selectedSignalSlot(int)));
	addItem(rect_object);
}

void EditScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QPointF p = event->scenePos();
	switch (selected_object_type) {
	case GamRectTag: {
		EditableRectObject *r = new EditableRectObject(p.x(), p.y(), 30, 30);
		r->setBrush(QColor("#00eeee"));
		addItem(r);
		addItem(r->top_left_corner);
		addItem(r->top_right_corner);
		addItem(r->bottom_left_corner);
		addItem(r->bottom_right_corner);
		addItem(r->editing_rect);
		connect(r, SIGNAL(selectedEditableSignal(GamObject *, int)),
				this, SLOT(selectedEditableSignalSlot(GamObject *, int)));
		selected_object_type = -1;
		QGraphicsEffect *e = rect_object->graphicsEffect();
		e->setEnabled(false);
		selected_editable_object = r;
		break;
	}
	case GamEllipseTag:
		break;
	case GamTextureTag:
		break;
	case GamTextTag:
		break;
	default: {
		QGraphicsItem *item = itemAt(p.x(), p.y());
		if (selected_editable_object && !item) {
			selected_editable_object->setEditable(false);
		}
		break;
	}
	}
	QGraphicsScene::mousePressEvent(event);
}

void EditScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{

	QGraphicsScene::mouseMoveEvent(event);
}

void EditScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{

	QGraphicsScene::mouseReleaseEvent(event);
}

void EditScene::selectedSignalSlot(int type)
{
	selected_object_type = type;
	if (selected_editable_object) {
		selected_editable_object->setEditable(false);
	}
}

void ColorDialog::hideEvent(QHideEvent *event)
{
	emit closedDialogSignal();
}

Menu::Menu(QListWidget *parent) : QListWidget(parent)
{
	setWindowFlags(windowFlags() | Qt::FramelessWindowHint);//Qt::Popup);
	QFont font("monaco");
	font.setPointSize(14);
	font.setFixedPitch(true);
	font.setBold(false);
	setFont(font);
	setFrameStyle(QFrame::Panel);
	setMidLineWidth(5);
	//resize(QSize(150, 150));
	setWindowOpacity(0.8);
	setMouseTracking(true);
	connect(this, SIGNAL(itemEntered(QListWidgetItem *)),
			this, SLOT(itemEnteredSlot(QListWidgetItem *)));
	connect(this, SIGNAL(itemDoubleClicked(QListWidgetItem *)),
			this, SLOT(itemDoubleClickedSlot(QListWidgetItem *)));
	connect(this, SIGNAL(itemClicked(QListWidgetItem *)),
			this, SLOT(itemClickedSlot(QListWidgetItem *)));

	QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
	cut = new QListWidgetItem();
	cut->setText("Cut          ⌘X");
	cut->setBackground(QBrush("black"));
	cut->setForeground(QBrush("white"));

	copy = new QListWidgetItem();
	copy->setText("Copy         ⌘C");
	copy->setBackground(QBrush("black"));
	copy->setForeground(QBrush("white"));

	paste = new QListWidgetItem();
	paste->setText("Paste        ⌘V");
	paste->setBackground(QBrush("black"));
	paste->setForeground(QBrush("white"));

	name = new QListWidgetItem();
	name->setData(Qt::StatusTipRole, "name");
	name->setText("name");
	name->setTextAlignment(Qt::AlignHCenter);
	name->setBackground(QBrush("#696969"));
	name->setForeground(QBrush("white"));
	name->setFlags(name->flags() & ~Qt::ItemIsSelectable);
	name_edit = new QListWidgetItem();
	name_edit->setFlags(name_edit->flags() | Qt::ItemIsEditable);
	name_edit->setData(Qt::EditRole, "value0");
	name_edit->setTextAlignment(Qt::AlignHCenter);
	name_edit->setBackground(QBrush("black"));
	name_edit->setForeground(QBrush("white"));

	color = new QListWidgetItem();
	color->setData(Qt::StatusTipRole, "color");
	color->setText("color");
	color->setTextAlignment(Qt::AlignHCenter);
	color->setBackground(QBrush("#696969"));
	color->setForeground(QBrush("white"));
	color->setFlags(name->flags() & ~Qt::ItemIsSelectable);
	color_dialog = new QListWidgetItem();
	color_dialog->setFlags(name_edit->flags() | Qt::ItemIsEditable);
	//color_dialog->setData(Qt::EditRole, "#00eeee");
	color_dialog->setText("#00eeee");
	color_dialog->setTextAlignment(Qt::AlignHCenter);
	color_dialog->setBackground(QBrush("black"));
	color_dialog->setForeground(QBrush("white"));

	physics = new QListWidgetItem();
	physics->setText("physics");
	physics->setFlags(physics->flags() | Qt::ItemIsUserCheckable);
	physics->setCheckState(Qt::Unchecked);
	physics->setTextAlignment(Qt::AlignHCenter);
	physics->setBackground(QBrush("black"));
	physics->setForeground(QBrush("white"));

	dialog = new ColorDialog();
	connect(dialog, SIGNAL(colorSelected(QColor)),
			this, SLOT(colorSelectedSlot(QColor)));
	addItem(cut);
	addItem(copy);
	addItem(paste);
	addItem(name);
	addItem(name_edit);
	addItem(color);
	addItem(color_dialog);
	addItem(physics);
}

void Menu::itemEnteredSlot(QListWidgetItem *item)
{
	setCurrentItem(item);
}

void Menu::itemDoubleClickedSlot(QListWidgetItem *item)
{
	editItem(item);
}

void Menu::colorSelectedSlot(QColor color)
{
	color_dialog->setText(color.name());
	emit colorSelectedSignal(color);
}

void Menu::itemClickedSlot(QListWidgetItem *item)
{
	if (item == color_dialog) {
		dialog->open();
	}
}

/*
void Menu::show(void)
{
	QListWidget::show();
}
*/
void EditScene::selectedEditableSignalSlot(GamObject *o, int type)
{
	selected_editable_object_type = type;
	switch (type) {
	case GamRectTag: {
		selected_editable_object = (EditableRectObject *)o;
		//EditableRectObject *r = (EditableRectObject *)o;
		//r->setEditable(true);
		break;
	}
	case GamEllipseTag:
		break;
	case GamTextureTag:
		break;
	case GamTextTag:
		break;
	default:
		break;
	}
}

RectObject::RectObject(int x, int y, int width, int height) : GamRect(x, y, width, height)
{
	QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect();
	effect->setColor(QColor("#ffd700"));
	effect->setOffset(0, 0);
	effect->setBlurRadius(50);
	effect->setEnabled(false);
	setGraphicsEffect(effect);
}

void RectObject::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	fprintf(stderr, "mousePress\n");
	QGraphicsEffect *e = graphicsEffect();
	e->setEnabled(true);
	emit selectedSignal(tag());
}

ResizeEllipseObject::ResizeEllipseObject(void)
{

}

void ResizeEllipseObject::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	emit resizeSignal(corner_type, event);
}

void ResizeEllipseObject::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	emit resizeSignal(corner_type, event);
}

void ResizeEllipseObject::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	emit resizeSignal(corner_type, event);
}

EditableRectObject::EditableRectObject(int x, int y, int width, int height) : GamRect(x, y, width, height)
{
	editing_rect = new GamRect(x, y, width, height);
	editing_rect->setBrush(QColor("#00eeee"));
	editing_rect->setOpacity(0.6);
	editing_rect->setVisible(false);
	editing_rect->setZValue(-1);
	top_left_corner = new ResizeEllipseObject();
	top_left_corner->setRectShape(&GamRect(x - 2, y - 2, 5, 5));
	top_left_corner->setBrush(QColor("#ee0000"));
	top_left_corner->setCursor(Qt::SizeFDiagCursor);
	top_left_corner->corner_type = TopLeftCorner;
	top_left_corner->setVisible(false);
	top_right_corner = new ResizeEllipseObject();
	top_right_corner->setRectShape(&GamRect(x + width - 3, y - 2, 5, 5));
	top_right_corner->setBrush(QColor("#ee0000"));
	top_right_corner->setCursor(Qt::SizeBDiagCursor);
	top_right_corner->corner_type = TopRightCorner;
	top_right_corner->setVisible(false);
	bottom_left_corner = new ResizeEllipseObject();
	bottom_left_corner->setRectShape(&GamRect(x - 2, y + height - 2, 5, 5));
	bottom_left_corner->setBrush(QColor("#ee0000"));
	bottom_left_corner->setCursor(Qt::SizeBDiagCursor);
	bottom_left_corner->corner_type = BottomLeftCorner;
	bottom_left_corner->setVisible(false);
	bottom_right_corner = new ResizeEllipseObject();
	bottom_right_corner->setRectShape(&GamRect(x + width - 3, y + height - 2, 5, 5));
	bottom_right_corner->setBrush(QColor("#ee0000"));
	bottom_right_corner->setCursor(Qt::SizeFDiagCursor);
	bottom_right_corner->corner_type = BottomRightCorner;
	bottom_right_corner->setVisible(false);
	connect(top_left_corner, SIGNAL(resizeSignal(CornerType, QGraphicsSceneMouseEvent *)),
			this, SLOT(resizeSignalSlot(CornerType, QGraphicsSceneMouseEvent *)));
	connect(top_right_corner, SIGNAL(resizeSignal(CornerType, QGraphicsSceneMouseEvent *)),
			this, SLOT(resizeSignalSlot(CornerType, QGraphicsSceneMouseEvent *)));
	connect(bottom_left_corner, SIGNAL(resizeSignal(CornerType, QGraphicsSceneMouseEvent *)),
			this, SLOT(resizeSignalSlot(CornerType, QGraphicsSceneMouseEvent *)));
	connect(bottom_right_corner, SIGNAL(resizeSignal(CornerType, QGraphicsSceneMouseEvent *)),
			this, SLOT(resizeSignalSlot(CornerType, QGraphicsSceneMouseEvent *)));
	menu = new Menu(NULL);
	connect(menu, SIGNAL(colorSelectedSignal(QColor)),
			this, SLOT(colorSelectedSignalSlot(QColor)));
	connect(menu->dialog, SIGNAL(currentColorChanged(QColor)),
			this, SLOT(currentColorChangedSlot(QColor)));
	connect(menu->dialog, SIGNAL(closedDialogSignal()),
			this, SLOT(closedDialogSignalSlot()));
}

void EditableRectObject::setEditable(bool b)
{
	if (!b) {
		QPen pen(Qt::SolidLine);
		pen.setColor(QColor("#000000"));
		pen.setWidth(1);
		setPen(pen);
		top_left_corner->setVisible(false);
		top_right_corner->setVisible(false);
		bottom_left_corner->setVisible(false);
		bottom_right_corner->setVisible(false);
		editing_rect->setBrush(QColor("#00eeee"));
		editing_rect->setOpacity(0.6);
		editing_rect->setVisible(false);
		editing_rect->setZValue(-1);
		menu->hide();
		menu->dialog->hide();
	}
}

void EditableRectObject::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QPen pen(Qt::SolidLine);
	pen.setColor(QColor("#ee0000"));
	pen.setWidth(2);
	setPen(pen);
	top_left_corner->setVisible(true);
	top_right_corner->setVisible(true);
	bottom_left_corner->setVisible(true);
	bottom_right_corner->setVisible(true);
	prev_pos = event->scenePos();
	emit selectedEditableSignal(this, tag());
	if (event->button() == Qt::RightButton) {
		QGraphicsView *v = scene()->views().at(0);
		QPointF pos = event->scenePos();
		menu->setGeometry(v->x() + pos.x(), v->y() + pos.y(), 150, 200);
		QPointF p = menu->pos();
		menu->show();
	}
}

void EditableRectObject::resizeSignalSlot(CornerType type, QGraphicsSceneMouseEvent *event)
{
	QPointF p = event->scenePos();
	if (event->type() == QEvent::GraphicsSceneMouseMove) {
		int dx = p.x() - prev_pos.x();
		int dy = p.y() - prev_pos.y();
		switch (type) {
		case TopLeftCorner:
			x += dx;
			y += dy;
			width -= dx;
			height -= dy;
			break;
		case TopRightCorner:
			y += dy;
			width += dx;
			height -= dy;
			break;
		case BottomLeftCorner:
			x += dx;
			width -= dx;
			height += dy;
			break;
		case BottomRightCorner:
			width += dx;
			height += dy;
			break;
		default:
			break;
		}
		editing_rect->setVisible(true);
		editing_rect->setZValue(1);
		editing_rect->setRect(x, y, width, height);
	} else if (event->type() == QEvent::GraphicsSceneMouseRelease) {
		editing_rect->setVisible(false);
		editing_rect->setZValue(-1);
		setRect(x, y, width, height);
		top_left_corner->setRectShape(&GamRect(x - 2, y - 2, 5, 5));
		top_right_corner->setRectShape(&GamRect(x + width - 3, y - 2, 5, 5));
		bottom_left_corner->setRectShape(&GamRect(x - 2, y + height - 2, 5, 5));
		bottom_right_corner->setRectShape(&GamRect(x + width - 3, y + height - 2, 5, 5));
	}
	prev_pos = p;
}

void EditableRectObject::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	editing_rect->setVisible(true);
	editing_rect->setZValue(1);
	QPointF p = event->scenePos();
	int dx = p.x() - prev_pos.x();
	int dy = p.y() - prev_pos.y();
	x += dx;
	y += dy;
	editing_rect->setRect(x, y, width, height);
	prev_pos = p;
}

void EditableRectObject::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	editing_rect->setVisible(false);
	editing_rect->setZValue(-1);
	setRect(x, y, width, height);
	top_left_corner->setRectShape(&GamRect(x - 2, y - 2, 5, 5));
	top_right_corner->setRectShape(&GamRect(x + width - 3, y - 2, 5, 5));
	bottom_left_corner->setRectShape(&GamRect(x - 2, y + height - 2, 5, 5));
	bottom_right_corner->setRectShape(&GamRect(x + width - 3, y + height - 2, 5, 5));
}

void EditableRectObject::colorSelectedSignalSlot(QColor color)
{
	setBrush(QBrush(color));
}

void EditableRectObject::currentColorChangedSlot(QColor color)
{
	editing_rect->setOpacity(opacity());
	editing_rect->setVisible(true);
	editing_rect->setZValue(1);
	editing_rect->setBrush(QBrush(color));
}

void EditableRectObject::closedDialogSignalSlot(void)
{
	editing_rect->setBrush(QColor("#00eeee"));
	editing_rect->setOpacity(0.6);
	editing_rect->setVisible(false);
	editing_rect->setZValue(-1);
}

int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	EditScene scene;
	QGraphicsView *view = new QGraphicsView(&scene);
	view->setRenderHint(QPainter::Antialiasing);
	view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
	//view.showFullScreen();
	view->show();
	scene.setSceneRect(0, 0, view->width(), view->height());
	//QuickEditor editor;
	//editor.show();
	app.exec();
	return 0;
}
