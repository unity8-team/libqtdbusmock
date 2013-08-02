/*
 * Copyright (C) 2013 Canonical, Ltd.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 3, as published
 * by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranties of
 * MERCHANTABILITY, SATISFACTORY QUALITY, or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Pete Woods <pete.woods@canonical.com>
 */

#include <qmenumodel/qdbusmenumodel.h>

#include <QCoreApplication>
#include <QDebug>

#include <WiFiMenu.h>

class MenuPrinter: public QDBusMenuModel {
Q_OBJECT

public:
	MenuPrinter(QObject* parent = 0) :
			QDBusMenuModel(parent) {
		setProperty("busType", DBusEnums::SessionBus);
		setProperty("busName", QString::fromStdString(MenuExporter::DBUS_NAME));
		setProperty("objectPath",
				QString::fromStdString(MenuExporter::DBUS_PATH));
		QObject::connect(this,
				SIGNAL(rowsInserted(const QModelIndex&, int, int)),
				SLOT(onModelChanged()));
	}

private Q_SLOTS:
	void onModelChanged() {
		printModel(this);
		qDebug() << "================";
	}

private:
	void printModel(QMenuModel* model, int indent = 0) {
		int count = model->rowCount();
		for (int i = 0; i < count; ++i) {
			QModelIndex index = model->index(i, 0);
			QString label = model->data(index, QMenuModel::Label).toString();
			QVariantMap extra = model->data(index, QMenuModel::Extra).toMap();
			{
				QDebug debug(qDebug().nospace());
				for (int j = 0; j < indent * 2; ++j)
					debug << " ";
				debug << " > " << label << " : " << extra;
			}
			printChilden(model, index, indent + 1);
		}
	}

	void printChilden(QMenuModel* model, QModelIndex &index, int indent = 0) {
		int children = model->rowCount(index);
		for (int i = 0; i < children; ++i) {
			QModelIndex childIndex(model->index(i, 0, index));
			QString label(
					model->data(childIndex, QMenuModel::Label).toString());
			QVariantMap extra(
					model->data(childIndex, QMenuModel::Extra).toMap());
			{
				QDebug debug(qDebug().nospace());
				for (int j = 0; j < indent * 2; ++j)
					debug << " ";
				debug << " > " << label << " : " << extra;
			}
			printChilden(model, childIndex, indent + 1);
		}
	}
};

#include "MenuPrinter.moc"

int main(int argc, char** argv) {
	QCoreApplication application(argc, argv);
	MenuPrinter monitor;
	monitor.start();
	int result = application.exec();
	monitor.stop();
	return result;
}
