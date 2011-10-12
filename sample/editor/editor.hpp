#include <gam.hpp>

class ColorDialog : public QColorDialog {
	Q_OBJECT;
public:
	void hideEvent(QHideEvent *event);
signals:
	void closedDialogSignal(void);
};

class Menu : public QListWidget {
	Q_OBJECT;
public:
	QListWidgetItem *cut;
	QListWidgetItem *copy;
	QListWidgetItem *paste;
	QListWidgetItem *name;
	QListWidgetItem *name_edit;
	QListWidgetItem *color;
	QListWidgetItem *color_dialog;
	QListWidgetItem *physics;
	ColorDialog *dialog;

	Menu(QListWidget *parent);
signals:
	void colorSelectedSignal(QColor color);
	void colorChangedSignal(QColor color);
public slots:
	void itemEnteredSlot(QListWidgetItem *item);
	void itemDoubleClickedSlot(QListWidgetItem *item);
	void itemClickedSlot(QListWidgetItem *item);
	void colorSelectedSlot(QColor color);
};

class RectObject : public GamRect {
	Q_OBJECT;
public:
	RectObject(int x, int y, int width, int height);
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
signals:
	void selectedSignal(int type);
};

typedef enum {
	TopLeftCorner,
	TopRightCorner,
	BottomLeftCorner,
	BottomRightCorner
} CornerType;

class ResizeEllipseObject : public GamEllipse {
	Q_OBJECT;
public:
	CornerType corner_type;

	ResizeEllipseObject(void);
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
signals:
	void resizeSignal(CornerType type, QGraphicsSceneMouseEvent *event);
};

class EditableRectObject : public GamRect {
	Q_OBJECT;
public:
	ResizeEllipseObject *top_left_corner;
	ResizeEllipseObject *top_right_corner;
	ResizeEllipseObject *bottom_left_corner;
	ResizeEllipseObject *bottom_right_corner;
	QPointF prev_pos;
	GamRect *editing_rect;
	Menu *menu;

	EditableRectObject(int x, int y, int width, int height);
	void setEditable(bool b);
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
signals:
	void selectedEditableSignal(GamObject *, int type);
public slots:
	void resizeSignalSlot(CornerType type, QGraphicsSceneMouseEvent *event);
	void colorSelectedSignalSlot(QColor color);
	void currentColorChangedSlot(QColor color);
	void closedDialogSignalSlot(void);
};

class EditScene : public GamScene {
	Q_OBJECT;
public:
	int selected_object_type;
	int selected_editable_object_type;
	RectObject *rect_object;
	EditableRectObject *selected_editable_object;

	EditScene(void);
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
public slots:
	void selectedSignalSlot(int type);
	void selectedEditableSignalSlot(GamObject *, int type);
};
