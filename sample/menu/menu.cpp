#include <QtGui>

int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	QWidget widget;
	widget.setWindowFlags(Qt::Popup);
	QListWidget list;
	QListWidgetItem item1("hoge");
	QListWidgetItem item2("hi");
	item1.setSizeHint(QSize(50, 30));
	item2.setSizeHint(QSize(50, 30));
	list.addItem(&item1);
	list.addItem(&item2);
	QFont font("monaco");
	font.setPointSize(14);
	font.setFixedPitch(true);
	font.setBold(false);
	list.setFont(font);
	//list.setMidLineWidth(1);
	//list.setGeometry(0, 0, 50, 100);
	QLineEdit line;
	QHBoxLayout layout;
	layout.addWidget(&list);
	layout.addWidget(&line);
	layout.setSpacing(0);
	layout.setContentsMargins(0, 0, 0, 0);
	layout.setGeometry(QRect(0, 0, 200, 100));
	widget.setLayout(&layout);
	widget.resize(QSize(200, 100));
	widget.show();
	return app.exec();
}
