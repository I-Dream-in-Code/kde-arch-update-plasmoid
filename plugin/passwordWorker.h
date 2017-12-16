#ifndef PASSWORD_WORKER_H
#define PASSWORD_WORKER_H
#include <QObject>
class PasswordWorker : public QObject
{
    Q_OBJECT
	
public:
	QString password;
public slots:
	void promptPassword();
};

#endif
