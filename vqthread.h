#ifndef VQTHREAD_H
#define VQTHREAD_H
#include <QThread>

class VQThread : public QThread
{
public:
    VQThread(QObject *parent = 0);
    ~VQThread();
protected:
    void run();
private:

};

#endif // VQTHREAD_H
