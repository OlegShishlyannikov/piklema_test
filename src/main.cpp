#include <cstdint>
#include <string>

#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "mqtt_client.hpp"

int32_t main(int32_t argc, char *argv[]){
  std::function<QObject*(QObject *, const QString &)> findObjectRecursive = [&findObjectRecursive](QObject* object, const QString& objectName) -> QObject *{
	if (object->objectName() == objectName) {
	  return object;
	}

	for (QObject* child : object->children()) {
	  QObject* foundObject = findObjectRecursive(child, objectName);
	  if (foundObject) {
		return foundObject;
	  }
	}

	return nullptr;
  };
  
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  QGuiApplication app(argc, argv);

  
  QQmlApplicationEngine engine;
  const QUrl url(QStringLiteral("qrc:/src/qml/main.qml"));
  QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
				   &app, [url](QObject *obj, const QUrl &objUrl) {
					 if (!obj && url == objUrl)
					   QCoreApplication::exit(-1);
				   }, Qt::QueuedConnection);
  engine.load(url);

  mqtt_client_s *mqtt_client = new mqtt_client_s;
  
  // Find out button object to set property
  QList<QObject*> rootObjects = engine.rootObjects();
  QObject* run_btn_obj = nullptr;
  
  for (QObject* rootObject : rootObjects) {
	run_btn_obj = findObjectRecursive(rootObject, "runButton");
  }
  
  if(!run_btn_obj){
	
	qFatal("RUN button hasn't been found, exiting");
	return -1;
  } else {

	run_btn_obj->setProperty("mqtt_client", QVariant::fromValue(mqtt_client));
  }

  return app.exec();
}
