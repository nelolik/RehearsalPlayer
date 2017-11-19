#include "mediaitem.h"

MediaItem::MediaItem() :
    fileName(""),
    filePath(""),
    timeLables(),
    isPlaying(false)
{

}

MediaItem::MediaItem(QString file_name, QString file_path) :
    fileName(file_name),
    filePath(file_path),
    timeLables(),
    isPlaying(false)
{

}
