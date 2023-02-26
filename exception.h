#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <QException>
#include <QString>

class Exception : public QException
{
public:
    Exception(const QString& msg= QString());
    virtual Exception *clone() const override { return new Exception(*this); };
    virtual void raise() const override final { throw *this; };

    virtual ~Exception() noexcept {};

    const QString& getMessage() const { return errorMessage; };
private:
    QString errorMessage;
};

#endif // EXCEPTION_H
