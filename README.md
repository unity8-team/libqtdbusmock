# Qt library to help easily mock DBus services

This library is designed to integrate with [libqtdbustest](https://github.com/pete-woods/libqtdbustest) in order to facilitate creating mocks of DBus services for use in your test fixtures. It's built on top of [python-dbusmock](https://github.com/martinpitt/python-dbusmock), which provides a number of built in templates for common Linux DBus services.


# Mocking DBus services

Start from the example in [libqtdbustest](https://github.com/pete-woods/libqtdbustest).

The `QtDBusMock::DBusMock` class facilitates running mock DBus services for testing your interactions against. There are a number of built-in mocks for some common Linux services to use, or you can build your own from scratch.

```cpp
class TestMyStuff: public ::testing::Test {
protected:
  TestMyStuff() : mock_(dbus_) {
    mock_.registerLogin1({{"DefaultSeat", QVariantMap
    {
      {"CanMultiSession", canMultiSession},
      {"CanGraphical", canGraphical}
    }}});

    dbus_.registerService(
        DBusServicePtr(new QProcessDBusService("org.freedesktop.MyBusName",
            QDBusConnection::SessionBus, "/path/to/executable/,
            QStringList{argument1, argument2})));

    dbus_.startServices();
  }

  QtDBusTest::DBusTestRunner dbus_;
  QtDBusMock::DBusMock mock_;
}
```
