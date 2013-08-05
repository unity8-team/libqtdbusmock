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

#ifndef LIBQTDBUSMOCK_METHODCALLS_H_
#define LIBQTDBUSMOCK_METHODCALLS_H_

#include <QtDBus>

namespace QtDBusMock {

class Q_DECL_EXPORT MethodCall {
public:
	explicit MethodCall();

	MethodCall(const MethodCall &other);

	MethodCall& operator=(const MethodCall &other);

	~MethodCall();

	static void registerMetaType();

	const quint64 & timestamp() const;

	const QVariantList & args() const;

	void setTimestamp(quint64 timestamp);

	void setArgs(const QVariantList &args);

protected:
	quint64 m_timestamp;

	QVariantList m_args;
};

}

QDBusArgument &operator<<(QDBusArgument &argument,
		const QtDBusMock::MethodCall &methodCalls);

const QDBusArgument &operator>>(const QDBusArgument &argument,
		QtDBusMock::MethodCall &methodCalls);

Q_DECLARE_METATYPE(QtDBusMock::MethodCall)

#endif // LIBQTDBUSMOCK_METHODCALLS_H_
