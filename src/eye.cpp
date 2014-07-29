#include "eye.h"

#include <QDir>
#include <QDebug>

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>

Eye::Eye(QObject *parent) :
    QCamera(parent)
{
}

Eye::Eye(const QByteArray &device, QObject *parent) :
    QCamera(device, parent)
{
}

Eye::~Eye()
{
}

/*
QList<QByteArray> Eye::m_rawDevices;
QStringList Eye::m_rawDescriptions;

QList<QByteArray> Eye::availableDevices()
{
    if( m_rawDevices.isEmpty() )
        updateRawDevices();

    return QCamera::availableDevices() + m_rawDevices;
}

QString Eye::deviceDescription(const QByteArray &device)
{
    QString desc = QCamera::deviceDescription(device);

    if( ! desc.isEmpty() ) {
        return desc;
    }

    if (m_rawDevices.isEmpty())
        updateRawDevices();

    for (int i=0; i<m_rawDevices.count(); i++)
        if (m_rawDevices[i] == device)
            return m_rawDescriptions[i];

    return QString();
}

void Eye::updateRawDevices()
{
    m_rawDevices.clear();
    m_rawDescriptions.clear();

    QDir devDir("/dev");
    devDir.setFilter(QDir::System);

    QFileInfoList entries = devDir.entryInfoList(QStringList() << "video*");

    foreach (const QFileInfo &entryInfo, entries) {
        int fd = ::open(entryInfo.filePath().toLatin1().constData(), O_RDWR );
        if (fd == -1)
            continue;

        bool isCamera = false;

        v4l2_input input;
        memset(&input, 0, sizeof(input));
        for (; ::ioctl(fd, VIDIOC_ENUMINPUT, &input) >= 0; ++input.index) {
            if (input.type == V4L2_INPUT_TYPE_CAMERA || input.type == 0) {
                isCamera = ::ioctl(fd, VIDIOC_S_INPUT, input.index) != 0;
                break;
            }
        }

        if (isCamera) {
            // find out its driver "name"
            QString name;
            struct v4l2_capability vcap;
            memset(&vcap, 0, sizeof(struct v4l2_capability));

            if (ioctl(fd, VIDIOC_QUERYCAP, &vcap) != 0)
                name = entryInfo.fileName();
            else
                name = QString((const char*)vcap.card);
            //qDebug() << "found camera: " << name;

            m_rawDevices.append(entryInfo.filePath().toLocal8Bit());
            m_rawDescriptions.append(name);
        }
        ::close(fd);
    }
}
*/
