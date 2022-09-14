#include "authhandler.h"
#include <QDebug>
#include <QVariantMap>
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkRequest>


AuthHandler::AuthHandler(QObject *parent)
    : QObject(parent)
    , m_apiKey(QString())
{

}

AuthHandler::~AuthHandler()
{
    m_networkAccessManager->deleteLater();
}

void AuthHandler::setAPIKey(const QString &apiKey)
{
    m_apiKey = apiKey;
}



void AuthHandler::signUserUp(const QString &emailAddress, const QString &password)
{
    m_responseType = "signUserUp";

    QString signUpEndpoint = "https://identitytoolkit.googleapis.com/v1/accounts:signUp?key=" + m_apiKey;

    QVariantMap variantPayload;
    variantPayload["email"] = emailAddress;
    variantPayload["password"] = password;
    variantPayload["returnSecureToken"] = true;

    QJsonDocument jsonPayload = QJsonDocument::fromVariant( variantPayload );
    performPost( signUpEndpoint, jsonPayload );
}

void AuthHandler::signUserIn(const QString &emailAddress, const QString &password)
{
    m_responseType = "signUserIn";

    QString signInEndpoint = "https://identitytoolkit.googleapis.com/v1/accounts:signInWithPassword?key=" + m_apiKey;

    QVariantMap variantPayload;
    variantPayload["email"] = emailAddress;
    variantPayload["password"] = password;
    variantPayload["returnSecureToken"] = true;

    QJsonDocument jsonPayload = QJsonDocument::fromVariant( variantPayload );
    performPost( signInEndpoint, jsonPayload );
}

void AuthHandler::sendPasswordResetEmail(const QString &emailAddress)
{
    m_responseType = "resetPassword";
    QString resetPasswordEndpoint = "https://identitytoolkit.googleapis.com/v1/accounts:sendOobCode?key=" + m_apiKey;
    QVariantMap variantPayload;
    variantPayload["email"] = emailAddress;
    variantPayload["requestType"] = "PASSWORD_RESET";
    QJsonDocument jsonPayload = QJsonDocument::fromVariant( variantPayload );
    performPost( resetPasswordEndpoint, jsonPayload );
}

void AuthHandler::createNetworkAccessManager()
{
    m_networkAccessManager = new QNetworkAccessManager( this );
    connect( this, &AuthHandler::userSignedIn, this, &AuthHandler::performAuthenticatedDatabaseCall );
}

void AuthHandler::networkReplyReadyRead()
{
    QByteArray response = m_networkReply->readAll();
    parseResponse(response);

    //qDebug()<< m_networkReply->readAll();


    m_networkReply->deleteLater();
}

void AuthHandler::performAuthenticatedDatabaseCall()
{
    QString endPoint = "https://qtapptest-3edb8-default-rtdb.firebaseio.com/FF.json?auth=" + m_idToken;
    m_networkReply = m_networkAccessManager->get(QNetworkRequest(QUrl(endPoint)));
    connect(m_networkReply, &QNetworkReply::readyRead,this,&AuthHandler::networkReplyReadyRead);
}


void AuthHandler::performPost(const QString &url, const QJsonDocument &payload)
{
    qDebug() << "perform post active";
    QNetworkRequest newRequest( (QUrl( url )) );
    newRequest.setHeader( QNetworkRequest::ContentTypeHeader, QString( "application/json"));
    m_networkReply = m_networkAccessManager->post( newRequest, payload.toJson());
    connect( m_networkReply, &QNetworkReply::readyRead, this, &AuthHandler::networkReplyReadyRead );
}


void AuthHandler::parseResponse(const QByteArray &response)
{
    QJsonDocument jsonDocument = QJsonDocument::fromJson(response);
    qDebug()<<"response:"<<response;
    if(jsonDocument.object().contains("error")){
        qDebug()<<"Error ocured!";
        if(m_responseType == "signUserUp"){
            qDebug()<<"userCreatedError!";
            emit userCreatedError();
        }
        else if(m_responseType == "signUserIn"){
            emit userSignInError();
        }else{
            emit resetPasswordError();
        }
    }else if(jsonDocument.object().contains("kind")){
        QString idToken = jsonDocument.object().value("idToken").toString();
        m_idToken = idToken;
        if(m_responseType == "signUserUp"){
            emit userCreated();
        }
        else if(m_responseType == "signUserIn"){
            emit userSignedIn();
        }else{
            emit resetPasswordSuccess();
        }
    }else{
        qDebug()<< "The response :" << response;
    }
}
