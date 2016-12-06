/*
 * Copyright (C) 2013 Canonical, Ltd.
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of version 3 of the GNU Lesser General Public License as published
 * by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Pete Woods <pete.woods@canonical.com>
 */

#pragma once

#include <QtDBus>

namespace QtDBusMock {

class Q_DECL_EXPORT Method {
public:
	explicit Method();

	Method(const Method &other);

	Method& operator=(const Method &other);

	~Method();

	static void registerMetaType();

	const QString & name() const;

	const QString & inSig() const;

	const QString & outSig() const;

	const QString & code() const;

	void setName(const QString &name);

	void setInSig(const QString &inSig);

	void setOutSig(const QString &outSig);

	void setCode(const QString &code);

protected:
	QString m_name;

	QString m_inSig;

	QString m_outSig;

	QString m_code;
};

}

Q_DECL_EXPORT
QDBusArgument &operator<<(QDBusArgument &argument,
		const QtDBusMock::Method &method);

Q_DECL_EXPORT
const QDBusArgument &operator>>(const QDBusArgument &argument,
		QtDBusMock::Method &method);

Q_DECLARE_METATYPE(QtDBusMock::Method)
