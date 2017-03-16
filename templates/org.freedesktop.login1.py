'''Login1 mock template
'''

# This program is free software; you can redistribute it and/or modify it under
# the terms of the GNU Lesser General Public License as published by the Free
# Software Foundation; either version 3 of the License, or (at your option) any
# later version.  See http://www.gnu.org/copyleft/lgpl.html for the full text
# of the license.

__author__ = 'Pete Woods'
__email__ = 'pete.woods@canonical.com'
__copyright__ = '(c) 2017 Canonical Ltd.'
__license__ = 'LGPL 3+'

import dbus
import uuid
import binascii

from dbusmock import MOCK_IFACE
import dbusmock


BUS_NAME = 'org.freedesktop.login1'
MAIN_IFACE = 'org.freedesktop.login1.Manager'
MAIN_OBJ = '/org/freedesktop/login1'
SYSTEM_BUS = True

SEAT_IFACE = 'org.freedesktop.login1.Seat'
SEAT_PATH = '/org/freedesktop/login1/seat/'

def load(mock, parameters):
    mock.seats = {}
    
    add_seat(mock, parameters.get('DefaultSeat', {}), True)
    
    mock.list_seats = list_seats
    mock.get_seat = get_seat
    
    mock.AddProperties(MAIN_IFACE, {
        'EnableWallMessages': parameters.get('EnableWallMessages', False),
        'WallMessage': parameters.get('WallMessage', ''),
        'NAutoVTs': parameters.get('NAutoVTs', dbus.UInt32(6)),
        'KillOnlyUsers': parameters.get('KillOnlyUsers', dbus.Array([], signature='s')),
        'KillExcludeUsers': parameters.get('KillExcludeUsers', dbus.Array(['root'], signature='s')),
        'KillUserProcesses': parameters.get('KillUserProcesses', False),
        'RebootToFirmwareSetup': parameters.get('RebootToFirmwareSetup', False),
        'IdleHint': parameters.get('IdleHint', False),
        'IdleSinceHint': parameters.get('IdleSinceHint', dbus.UInt64(1489580452083866)),
        'IdleSinceHintMonotonic': parameters.get('IdleSinceHintMonotonic', dbus.UInt64(8134597164)),
        'BlockInhibited': parameters.get('BlockInhibited', 'handle-power-key:handle-suspend-key:handle-hibernate-key:handle-lid-switch'),
        'DelayInhibited': parameters.get('DelayInhibited', 'sleep'),
        'InhibitDelayMaxUSec': parameters.get('InhibitDelayMaxUSec', dbus.UInt64(5000000)),
        'HandlePowerKey': parameters.get('HandlePowerKey', 'poweroff'),
        'HandleSuspendKey': parameters.get('HandleSuspendKey', 'suspend'),
        'HandleHibernateKey': parameters.get('HandleHibernateKey', 'hibernate'),
        'HandleLidSwitch': parameters.get('HandleLidSwitch', 'suspend'),
        'HandleLidSwitchDocked': parameters.get('HandleLidSwitchDocked', 'ignore'),
        'HoldoffTimeoutUSec': parameters.get('HoldoffTimeoutUSec', dbus.UInt64(30000000)),
        'IdleAction': parameters.get('IdleAction', 'ignore'),
        'IdleActionUSec': parameters.get('IdleActionUSec', dbus.UInt64(1800000000)),
        'PreparingForShutdown': parameters.get('PreparingForShutdown', False),
        'PreparingForSleep': parameters.get('PreparingForSleep', False),
        'ScheduledShutdown': parameters.get('ScheduledShutdown', dbus.Struct(('', 0), signature='st')),
        'Docked': parameters.get('Docked', False),
    })

    mock.AddMethods(MAIN_IFACE, [
        ('GetSession', 's', 'o', "ret = dbus.ObjectPath('/')"),
        ('GetSessionByPID', 'u', 'o', "ret = dbus.ObjectPath('/')"),
        ('GetUser', 'u', 'o', "ret = dbus.ObjectPath('/')"),
        ('GetUserByPID', 'u', 'o', "ret = dbus.ObjectPath('/')"),
        ('GetSeat', 's', 'o', "ret = self.get_seat(self, args[0])"),

        ('ListSessions', '', 'a(susso)', ""),
        ('ListUsers', '', 'a(uso)', ""),
        ('ListSeats', '', 'a(so)', "ret = self.list_seats(self)"),
        ('ListInhibitors', '', 'a(ssssuu)', ""),

        ('CreateSession', 'uusssssussbssa(sv)', 'soshusub', ""),
        ('ReleaseSession', 's', '', ""),
        ('ActivateSession', 's', '', ""),
        ('ActivateSessionOnSeat', 'ss', '', ""),
        ('LockSession', 's', '', ""),
        ('UnlockSession', 's', '', ""),
        ('LockSessions', '', '', ""),
        ('UnlockSessions', '', '', ""),
        ('KillSession', 'ssi', '', ""),
        ('KillUser', 'ui', '', ""),
        ('TerminateSession', 's', '', ""),
        ('TerminateUser', 'u', '', ""),
        ('TerminateSeat', 's', '', ""),
        ('SetUserLinger', 'ubb', '', ""),
        ('AttachDevice', 'ssb', '', ""),
        ('FlushDevices', 'b', '', ""),
        ('PowerOff', 'b', '', ""),
        ('Reboot', 'b', '', ""),
        ('Suspend', 'b', '', ""),
        ('Hibernate', 'b', '', ""),
        ('HybridSleep', 'b', '', ""),
        ('CanPowerOff', '', 's', "ret = ''"),
        ('CanReboot', '', 's', "ret = ''"),
        ('CanSuspend', '', 's', "ret = ''"),
        ('CanHibernate', '', 's', "ret = ''"),
        ('CanHybridSleep', '', 's', "ret = ''"),
        ('ScheduleShutdown', 'st', '', ""),
        ('CancelScheduledShutdown', '', 'b', "ret = False"),
        ('Inhibit', 'ssss', 'h', "ret = ?"),
        ('CanRebootToFirmwareSetup', '', 's', "ret = ''"),
        ('SetRebootToFirmwareSetup', 'b', '', ""),
        ('SetWallMessage', 'sb', '', ""),
    ])

def add_seat(self, properties, is_self):
    # find a new name
    count = 0
    while True:
        id = 'seat%d' % count
        if id not in self.seats:
            break
        count += 1

    path = dbus.ObjectPath(SEAT_PATH + id)

    seat_props = {
        'Id': id,
        'ActiveSession': properties.get('ActiveSession', dbus.Struct(('c2', '/org/freedesktop/login1/session/c2'), signature='so')),
        'CanMultiSession': properties.get('CanMultiSession', True),
        'CanTTY': properties.get('CanTTY', True),
        'CanGraphical': properties.get('CanGraphical', True),
        'Sessions': properties.get('Sessions', dbus.Array([('c2', '/org/freedesktop/login1/session/c2')], signature='(so)')),
        'IdleHint': properties.get('IdleHint', False),
        'IdleSinceHint': properties.get('IdleSinceHint', dbus.UInt64(1489590854604069)),
        'IdleSinceHintMonotonic': properties.get('IdleSinceHintMonotonic', dbus.UInt64(18537117367)),
    }

    seat_methods = [
        ('Terminate', '', '', ""),
        ('ActivateSession', 's', '', ""),
        ('SwitchTo', 'u', '', ""),
        ('SwitchToNext', '', '', ""),
        ('SwitchToPrevious', '', '', ""),
    ]

    self.seats[id] = self.AddObject(path,
                   SEAT_IFACE,
                   seat_props,
                   seat_methods)

    if is_self:
        self_path = dbus.ObjectPath(SEAT_PATH + 'self')
        try:
            self.RemoveObject(self_path)
        except dbus.exceptions.DBusException:
            pass
        self.AddObject(self_path,
                       SEAT_IFACE,
                       seat_props,
                       seat_methods)

    return path

def get_seat(self, id):
    if id in self.seats:
        return SEAT_PATH + id
    raise dbus.exceptions.DBusException()

def list_seats(self):
    result = dbus.Array([], signature='(so)')
    for id in self.seats:
        result.append((id, SEAT_PATH + id))
    return result
