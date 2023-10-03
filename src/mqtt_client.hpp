#ifndef MQTT_CLIENT_HPP
#define MQTT_CLIENT_HPP

#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMqttClient>
#include <condition_variable>
#include <mutex>

class mqtt_client_s : public QMqttClient {
  Q_OBJECT;
  Q_PROPERTY(bool busy MEMBER busy_ NOTIFY busyChanged);

public:
  
  explicit mqtt_client_s(QObject *parent = nullptr);
  virtual ~mqtt_client_s() = default;
  
  Q_INVOKABLE void publish_file(const QJsonObject &, const QString &);

private:
  static constexpr auto publish_timeout_s_ = 1, connect_timeout_s_ = 2;
  std::condition_variable cv_;
  std::mutex mtx_;
  mutable bool payload_sent_, busy_, ready_;
  
signals:
  void transmitCompleted(int32_t);
  void busyChanged(bool);
};

#endif // MQTT_CLIENT_HPP
