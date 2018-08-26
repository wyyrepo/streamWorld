#ifndef VLC_H
#define VLC_H

/*****************************************************
 * VLC类
 * 1.该类是对LibVLC库的封装。VLC作为后台进行媒体的播放与控制，
 *   统一封装到该类中更方便管理。
 * 2.整个程序中只有一个VLC对象，音频与视频模式共用。而且该对象只
 *   与System类进行交流，并不直接与音频和视频窗口联系。
 ****************************************************/

#include <vlc/vlc.h>
#include <QWidget>

class Vlc
{
public:
    Vlc();
    ~Vlc();

    void setCache(int cache);               //设置缓冲时间。
    void setMedia(char *address, WId id);   //设置频道的方法，参数一是char ×类型的频道地址，参数二是窗口ID，音频模式设置为NULL。
    void setId(WId id);                     //设置窗口ID,Qt的窗口可以通过调用winId()方法获得该ID。
    void setVolume(int volume);             //设置音量。

    char *getUrl();    //获得当前正在播放的频道地址。

    //播放控制方法：播放、暂停、停止
    void play();
    void pause();
    void stop();

private:
    //vlc指针。
    libvlc_instance_t *base;
    libvlc_media_t *media;
    libvlc_media_player_t *player;

    //缓冲时间。
    int cache;

};

#endif // VLC_H
