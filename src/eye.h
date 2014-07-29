#ifndef EYE_H
#define EYE_H

#include <QCamera>

class Eye
    : public QCamera
{
    Q_OBJECT

public:
    explicit Eye(QObject *parent = 0);
    Eye(const QByteArray& device, QObject *parent = 0);
    ~Eye();

private:
    //static QList<QByteArray> m_rawDevices;
    //static QStringList       m_rawDescriptions;

signals:
    //void mediaObjectChanged();

public slots:

};

#endif // EYE_H
