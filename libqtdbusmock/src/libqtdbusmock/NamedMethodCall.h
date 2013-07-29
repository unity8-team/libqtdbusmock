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

#ifndef LIBQTDBUSMOCK_NAMEDMETHODCALL_H_
#define LIBQTDBUSMOCK_NAMEDMETHODCALL_H_

#include <QtDBus>

namespace QtDBusMock {

class NamedMethodCall {
public:
	explicit NamedMethodCall();

	NamedMethodCall(const NamedMethodCall &other);

	NamedMethodCall& operator=(const NamedMethodCall &other);

	~NamedMethodCall();

	static void registerMetaType();

	const QString & methodName() const;

	void setMethodName(const QString &methodName);

	const quint64 & timestamp() const;

	const QVariantList & args() const;

	void setTimestamp(quint64 timestamp);

	void setArgs(const QVariantList &args);

protected:
	quint64 m_timestamp;

	QString m_methodName;

	QVariantList m_args;
};

}

QDBusArgument &operator<<(QDBusArgument &argument,
		const QtDBusMock::NamedMethodCall &methodCalls);

const QDBusArgument &operator>>(const QDBusArgument &argument,
		QtDBusMock::NamedMethodCall &methodCalls);

Q_DECLARE_METATYPE(QtDBusMock::NamedMethodCall)

#endif // LIBQTDBUSMOCK_NAMEDMETHODCALL_H_
