#ifndef AUDIO_H
#define AUDIO_H
#include <QThread>

class VAudioThread : public QThread
{
public:
    VAudioThread();
    ~VAudioThread();
protected:
    void run();
};

#endif // AUDIO_H
