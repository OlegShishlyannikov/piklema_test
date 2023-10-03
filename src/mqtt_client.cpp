#include <QDebug>
#include <QFile>

#include "mqtt_client.hpp"
#include <thread>

mqtt_client_s::mqtt_client_s(QObject *parent) : QMqttClient(parent), payload_sent_(false), busy_(false), ready_(false){}

void mqtt_client_s::publish_file(const QJsonObject &settings, const QString &payload_file) {
  static int32_t published_strcnt = 0, read_strcnt = 0;
  
  busy_ = true;
  emit busyChanged(busy_);

  // Subscribe
  if(!settings.contains("host") || !settings.contains("port") || !settings.contains("username") || !settings.contains("password") || !settings.contains("topic")){
	qWarning("Bad settings");
	return;
  }

  // Read settings
  QString host = settings["host"].toString();
  uint16_t port = settings["port"].toString().toInt();
  QString username = settings["username"].toString();
  QString password = settings["password"].toString();
  QString topic = settings["topic"].toString();

  if(host.isEmpty() || !port || topic.isEmpty()){
	qWarning("Bad settings");
	return;	
  }
  
  // Connect temperary slots
  connect(static_cast<QMqttClient *>(this), &QMqttClient::messageReceived, this, [this](const QByteArray &message, const QMqttTopicName &topic) -> void {
	qInfo() << QString("[%1] Received message: '%2' from topic '%3'").arg(QDateTime::currentDateTime().toString(), message, topic.name());
  });

  connect(static_cast<QMqttClient *>(this), &QMqttClient::connected, this,
		  [this, p_published_strcnt = &published_strcnt, p_read_strcnt = &read_strcnt, payload_file, topic = settings["topic"].toString()]() -> void {
			// published_strcnt = 0;
			// read_strcnt = 0;
			
			QFile file(payload_file);
			file.open(QIODevice::ReadOnly);

			if (!file.isOpen()){
			  unsubscribe(topic);
			  disconnectFromHost();

			  busy_ = false;
			  emit busyChanged(busy_);

			  qWarning() << QString("Error opening file '%1'").arg(payload_file);
			  return;
			}

			QTextStream stream(&file);

			for (QString line = stream.readLine();
				 !line.isNull();
				 line = stream.readLine()) {
			  (*p_read_strcnt) ++;
			  publish(topic, line.toUtf8(), 1);
			};
		  });

  connect(static_cast<QMqttClient *>(this), &QMqttClient::disconnected, this, [this]() -> void {
	disconnect();	
	busy_ = false;
	emit busyChanged(busy_);
  });

  connect(static_cast<QMqttClient *>(this), &QMqttClient::messageSent, this,
		  [this, p_published_strcnt = &published_strcnt, p_read_strcnt = &read_strcnt, payload_file, topic = settings["topic"].toString()](int32_t msg_id) -> void {
			(*p_published_strcnt)++;

			if((*p_published_strcnt) == (*p_read_strcnt)){
			  unsubscribe(topic);
			  disconnectFromHost();

			  busy_ = false;
			  emit busyChanged(busy_);

			  qInfo() << QString("Published %1 strings").arg(QString::number((*p_published_strcnt)));
			}	
		  });  

  setHostname(host);
  setPort(port);
  setUsername(username);
  setPassword(password);
  connectToHost();  
}
