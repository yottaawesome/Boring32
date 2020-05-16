#pragma once
#include <Windows.h>
#include <string>
#include <vector>
#include "WinHttpHandle.hpp"
#include "WinHttpWebSocketStatus.hpp"

namespace Boring32::WinHttp
{
	class WinHttpWebSocket
	{
		public:
			virtual ~WinHttpWebSocket();
			WinHttpWebSocket(std::wstring server, const UINT port, const bool ignoreSslErrors);
			virtual const std::wstring& GetServer();
			virtual void Connect();
			virtual void Connect(const std::wstring& path);
			virtual void CleanServerString();
			virtual void SendString(const std::string& msg);
			virtual void SendBuffer(const std::vector<char>& buffer);
			virtual bool Receive(std::vector<char>& buffer);
			virtual void SetServer(const std::wstring& newServer, const UINT port, const bool ignoreSslErrors);
			virtual void Close();
			virtual WinHttpWebSocketStatus GetStatus();

		protected:
			virtual void InternalConnect(const std::wstring& path);

		protected:
			WinHttpHandle m_hConnect;
			WinHttpHandle m_hSession;
			WinHttpHandle m_webSocketHandle;
			std::wstring m_server;
			UINT m_port;
			bool m_ignoreSslErrors;
			WinHttpWebSocketStatus m_status;
	};
}
