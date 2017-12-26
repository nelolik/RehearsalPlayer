#ifndef MEDIAITEM_H
#define MEDIAITEM_H

#include <QList>

class QString;
class QByteArray;

class MediaItem
{
public:
    QString fileName;
    QString filePath;
    QList<int> timeLables;
    bool isPlaying;
public:
    MediaItem();
    MediaItem(QString file_name, QString file_path);
    MediaItem(QString file_name, QString file_path, bool is_playing, QList<int> &labels);
    QByteArray serialize();
    static MediaItem deserialize(QByteArray&);
};

#endif // MEDIAITEM_H
