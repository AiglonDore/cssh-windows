#ifndef LOOKUP_H
#define LOOKUP_H
#include <QString>
#ifdef _WIN32
bool sshDetected(const QString& appName = "ssh.exe");
#else
bool sshDetected(const QString& appName = "ssh");
#endif // _WIN32

#endif // LOOKUP_H
