#ifndef MEDIAITEM_H
#define MEDIAITEM_H

#include <QList>

class QString;

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
};

#endif // MEDIAITEM_H
