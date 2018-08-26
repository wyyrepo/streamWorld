#include "vlc.h"
#include <QDebug>

Vlc::Vlc()
{
    base = libvlc_new(0, NULL);
    media = NULL;
    player = libvlc_media_player_new(base);
    cache = 2000;
}

Vlc::~Vlc()
{
    libvlc_media_player_release(player);
    libvlc_media_release(media);
    libvlc_release(base);
}

void Vlc::setCache(int cache)
{
    this->cache = cache;
}

void Vlc::setMedia(char *address, WId id)
{
    libvlc_media_release(media);
    media = libvlc_media_new_location(base, address);
    libvlc_media_player_set_media(player, media);
    if (id == 0)
        libvlc_media_player_set_hwnd(player, NULL);
    else
        libvlc_media_player_set_hwnd(player, (void *)id);
}

void Vlc::setId(WId id)
{
    libvlc_media_player_set_hwnd(player, (void *)id);
}

void Vlc::setVolume(int volume)
{
    libvlc_audio_set_volume(player, volume);
}

char *Vlc::getUrl()
{
    return libvlc_media_get_mrl(media);
}

void Vlc::play()
{
    //获得char ×类型的缓冲时间参数。
    QString para = "network-caching=" + QString::number(cache);
    QByteArray ba = para.toLatin1();
    char *add = ba.data();
    libvlc_media_add_option(media, add);
    libvlc_media_player_play(player);
}

void Vlc::pause()
{
    libvlc_media_player_pause(player);
}

void Vlc::stop()
{
    libvlc_media_player_stop(player);
}
