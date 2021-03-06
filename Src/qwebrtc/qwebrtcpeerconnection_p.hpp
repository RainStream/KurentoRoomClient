#pragma once

#include <webrtc/api/peerconnectioninterface.h>
#include <webrtc/api/jsep.h>
//#include <webrtc/base/refcount.h>
#include <QList>
#include <QVariantMap>
#include "qwebrtcsessiondescription.hpp"
#include "qwebrtcpeerconnection.hpp"
#include "qwebrtctypes_p.hpp"
#include <QDebug>

class QWebRTCPeerConnection_impl;
class QWebRTCCreateSessionDescriptionObserver_p : public webrtc::CreateSessionDescriptionObserver {
public:
    virtual void OnSuccess(webrtc::SessionDescriptionInterface* desc) override;
    virtual void OnFailure(const std::string& error) override;
    void invokeHandler(const QSharedPointer<QWebRTCSessionDescription>&);
    //std::weak_ptr<QWebRTCPeerConnection_impl> impl;
    std::function<void(const QSharedPointer<QWebRTCSessionDescription>&)> m_completionHandler;
};

class QWebRTCSetSessionDescriptionObserver_p : public webrtc::SetSessionDescriptionObserver {
public:
    virtual void OnSuccess() override;
    virtual void OnFailure(const std::string& error) override;
    void invokeHandler(bool success);
//    std::weak_ptr<QWebRTCPeerConnection_impl> impl;
    std::function<void(bool)> m_completionHandler;
    bool m_remoteDescription = false;
    QSharedPointer<QWebRTCSessionDescription> m_description;
};

class QWebRTCPeerConnectionFactory_impl;
class QWebRTCPeerConnection_impl : public webrtc::PeerConnectionObserver {
public:
    explicit QWebRTCPeerConnection_impl(QWebRTCPeerConnection* q_ptr);
    virtual ~QWebRTCPeerConnection_impl();

    virtual void OnSignalingChange(webrtc::PeerConnectionInterface::SignalingState new_state) override;

    virtual void OnAddStream(rtc::scoped_refptr<webrtc::MediaStreamInterface> stream) override;

    virtual void OnRemoveStream(rtc::scoped_refptr<webrtc::MediaStreamInterface> stream) override;

    virtual void OnDataChannel(rtc::scoped_refptr<webrtc::DataChannelInterface> data_channel) override;

    virtual void OnRenegotiationNeeded() override;

    virtual void OnIceConnectionChange(webrtc::PeerConnectionInterface::IceConnectionState new_state) override;

    virtual void OnIceGatheringChange(webrtc::PeerConnectionInterface::IceGatheringState new_state) override;

    virtual void OnIceCandidate(const webrtc::IceCandidateInterface* candidate) override;

    virtual void OnIceCandidatesRemoved(const std::vector<cricket::Candidate>& candidates) override;

    virtual void OnIceConnectionReceivingChange(bool receiving) override;

    virtual void OnAddTrack(rtc::scoped_refptr<webrtc::RtpReceiverInterface> receiver,
        const std::vector<rtc::scoped_refptr<webrtc::MediaStreamInterface>>& streams)  override;

    rtc::scoped_refptr<webrtc::PeerConnectionInterface> _conn;

    QSharedPointer<QWebRTCSessionDescription> m_remoteDescription;
    QSharedPointer<QWebRTCSessionDescription> m_localDescription;

    //We need to keep the factory around till all peer connections are destroyed.
    QSharedPointer<QWebRTCPeerConnectionFactory_impl> m_factory;

    QWebRTCPeerConnection* q_ptr;
};
