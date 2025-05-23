/*
 * This file is part of the TrinityCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TRINITYCORE_WORLD_SOCKET_H
#define TRINITYCORE_WORLD_SOCKET_H

#include "AsyncCallbackProcessor.h"
#include "AuthDefines.h"
#include "DatabaseEnvFwd.h"
#include "MessageBuffer.h"
#include "Socket.h"
#include "WorldPacket.h"
#include "WorldPacketCrypt.h"
#include "MPSCQueue.h"
#include <array>
#include <boost/asio/ip/tcp.hpp>
#include <mutex>

namespace JSON::RealmList
{
class RealmJoinTicket;
}

typedef struct z_stream_s z_stream;
class EncryptablePacket;
class WorldPacket;
class WorldSession;
enum ConnectionType : int8;
enum OpcodeClient : uint32;

class EncryptablePacket : public WorldPacket
{
public:
    EncryptablePacket(WorldPacket const& packet, bool encrypt) : WorldPacket(packet), _encrypt(encrypt)
    {
        SocketQueueLink.store(nullptr, std::memory_order_relaxed);
    }

    bool NeedsEncryption() const { return _encrypt; }

    std::atomic<EncryptablePacket*> SocketQueueLink;

private:
    bool _encrypt;
};

namespace WorldPackets
{
    class ServerPacket;
    namespace Auth
    {
        class AuthSession;
        class AuthContinuedSession;
        class ConnectToFailed;
        class Ping;
    }
}

#pragma pack(push, 1)

struct PacketHeader
{
    uint32 Size;
    uint8 Tag[12];

    bool IsValidSize() const { return Size < 0x10000; }
};

struct IncomingPacketHeader : PacketHeader
{
    uint32 EncryptedOpcode;
};

#pragma pack(pop)

class TC_GAME_API WorldSocket final : public Trinity::Net::Socket<>
{
    static uint32 const MinSizeForCompression;

    static std::array<uint8, 32> const AuthCheckSeed;
    static std::array<uint8, 32> const SessionKeySeed;
    static std::array<uint8, 32> const ContinuedSessionSeed;
    static std::array<uint8, 32> const EncryptionKeySeed;

    using BaseSocket = Socket;

public:
    WorldSocket(Trinity::Net::IoContextTcpSocket&& socket);
    ~WorldSocket();

    WorldSocket(WorldSocket const& right) = delete;
    WorldSocket(WorldSocket&& right) = delete;
    WorldSocket& operator=(WorldSocket const& right) = delete;
    WorldSocket& operator=(WorldSocket&& right) = delete;

    void Start() override;
    bool Update() override;

    void SendPacket(WorldPacket const& packet);

    ConnectionType GetConnectionType() const { return _type; }

    void SendAuthResponseError(uint32 code);
    void SetWorldSession(WorldSession* session);
    void SetSendBufferSize(std::size_t sendBufferSize) { _sendBufferSize = sendBufferSize; }

    void OnClose() override;
    Trinity::Net::SocketReadCallbackResult ReadHandler() override;

    void QueueQuery(QueryCallback&& queryCallback);

    void SendAuthSession();
    bool InitializeCompression();

protected:
    bool ReadHeaderHandler();

    enum class ReadDataHandlerResult
    {
        Ok = 0,
        Error = 1,
        WaitingForQuery = 2
    };

    ReadDataHandlerResult ReadDataHandler();
private:
    /// writes network.opcode log
    /// accessing WorldSession is not threadsafe, only do it when holding _worldSessionLock
    void LogOpcodeText(OpcodeClient opcode, std::unique_lock<std::mutex> const& guard) const;
    /// sends and logs network.opcode without accessing WorldSession
    void SendPacketAndLogOpcode(WorldPacket const& packet);
    void WritePacketToBuffer(EncryptablePacket const& packet, MessageBuffer& buffer);
    uint32 CompressPacket(uint8* buffer, WorldPacket const& packet);

    void HandleAuthSession(std::shared_ptr<WorldPackets::Auth::AuthSession> authSession);
    void HandleAuthSessionCallback(std::shared_ptr<WorldPackets::Auth::AuthSession> authSession,
        std::shared_ptr<JSON::RealmList::RealmJoinTicket> joinTicket, PreparedQueryResult result);
    void HandleAuthContinuedSession(std::shared_ptr<WorldPackets::Auth::AuthContinuedSession> authSession);
    void HandleAuthContinuedSessionCallback(std::shared_ptr<WorldPackets::Auth::AuthContinuedSession> authSession, PreparedQueryResult result);
    void LoadSessionPermissionsCallback(PreparedQueryResult result);
    void HandleConnectToFailed(WorldPackets::Auth::ConnectToFailed& connectToFailed);
    bool HandlePing(WorldPackets::Auth::Ping& ping);
    void HandleEnterEncryptedModeAck();

    ConnectionType _type;
    uint64 _key;

    std::array<uint8, 32> _serverChallenge;
    WorldPacketCrypt _authCrypt;
    SessionKey _sessionKey;
    std::array<uint8, 32> _encryptKey;

    TimePoint _LastPingTime;
    uint32 _OverSpeedPings;

    std::mutex _worldSessionLock;
    WorldSession* _worldSession;
    bool _authed;
    bool _canRequestHotfixes;

    MessageBuffer _headerBuffer;
    MessageBuffer _packetBuffer;
    MPSCQueue<EncryptablePacket, &EncryptablePacket::SocketQueueLink> _bufferQueue;
    std::size_t _sendBufferSize;

    z_stream* _compressionStream;

    QueryCallbackProcessor _queryProcessor;
    std::string _ipCountry;
};

#endif
