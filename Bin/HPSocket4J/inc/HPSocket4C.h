/*
 * Copyright: JessMA Open Source (ldcsaa@gmail.com)
 *
 * Version	: 3.2.1
 * Author	: Bruce Liang
 * Website	: http://www.jessma.org
 * Project	: https://github.com/ldcsaa
 * Blog		: http://www.cnblogs.com/ldcsaa
 * Wiki		: http://www.oschina.net/p/hp-socket
 * QQ Group	: 75375912
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/******************************************************************************
Module:  HPSocket for C DLL

Desc: 导出纯 C 函数，让其它语言（如：C / C# / Delphi 等）能方便地使用 HPSocket

Usage:
		方法一：
		--------------------------------------------------------------------------------------
		0. （C/C++ 程序）包含 HPSocket4C.h 头文件
		1. 调用 ::Create_HP_XxxListener() 函数创建监听器对象
		2. 调用 ::Create_HP_Xxx(pListener) 函数创建 HPSocket 对象
		3. 调用 ::HP_Set_FN_Xxx_OnYyy(pListener, ...) 函数设置监听器的回调函数
		4. 调用相关导出函数操作 HPSocket 对象
		5. ...... ......
		6. 调用 ::Destroy_HP_Xxx(pSocket) 函数销毁 HPSocket 对象
		7. 调用 ::Destroy_HP_XxxListener(pListener) 函数销毁监听器对象

		方法二：
		--------------------------------------------------------------------------------------
		1. 应用程序把需要用到的导出函数封装到特定语言的包装类中
		2. 通过包装类封装后，以面向对象的方式使用 HPSocket

Release:
		1. x86/HPSocket4C.dll		- (32位/MBCS/Release)
		2. x86/HPSocket4C_D.dll		- (32位/MBCS/DeBug)
		3. x86/HPSocket4C_U.dll		- (32位/UNICODE/Release)
		4. x86/HPSocket4C_UD.dll	- (32位/UNICODE/DeBug)
		5. x64/HPSocket4C.dll		- (64位/MBCS/Release)
		6. x64/HPSocket4C_D.dll		- (64位/MBCS/DeBug)
		7. x64/HPSocket4C_U.dll		- (64位/UNICODE/Release)
		8. x64/HPSocket4C_UD.dll	- (64位/UNICODE/DeBug)

******************************************************************************/

#pragma once

/**************************************************/
/********* imports / exports HPSocket.dll *********/

#ifdef HPSOCKET_EXPORTS
	#define HPSOCKET_API extern "C" __declspec(dllexport)
#else
	#define HPSOCKET_API extern "C" __declspec(dllimport)
#endif

/************************************************************************
名称：连接 ID 数据类型
描述：定义连接 ID 的数据类型
************************************************************************/
typedef ULONG_PTR	HP_CONNID;

/************************************************************************
名称：定义 Socket 对象指针类型别名
描述：把 Socket 对象指针定义为更直观的别名
************************************************************************/

typedef PVOID		HP_Object;

typedef HP_Object	HP_Server;
typedef HP_Object	HP_Client;
typedef HP_Object	HP_Agent;
typedef HP_Object	HP_TcpServer;
typedef HP_Object	HP_TcpClient;
typedef HP_Object	HP_TcpAgent;
typedef HP_Object	HP_TcpPullServer;
typedef HP_Object	HP_TcpPullClient;
typedef HP_Object	HP_TcpPullAgent;
typedef HP_Object	HP_UdpServer;
typedef HP_Object	HP_UdpClient;

typedef HP_Object	HP_Listener;
typedef HP_Object	HP_ServerListener;
typedef HP_Object	HP_ClientListener;
typedef HP_Object	HP_AgentListener;
typedef HP_Object	HP_TcpServerListener;
typedef HP_Object	HP_TcpClientListener;
typedef HP_Object	HP_TcpAgentListener;
typedef HP_Object	HP_TcpPullServerListener;
typedef HP_Object	HP_TcpPullClientListener;
typedef HP_Object	HP_TcpPullAgentListener;
typedef HP_Object	HP_UdpServerListener;
typedef HP_Object	HP_UdpClientListener;

/*****************************************************************************************************/
/******************************************** 公共类、接口 ********************************************/
/*****************************************************************************************************/

/************************************************************************
名称：通信组件服务状态
描述：应用程序可以通过通信组件的 GetState() 方法获取组件当前服务状态
************************************************************************/
enum En_HP_ServiceState
{
	HP_SS_STARTING	= 0,	// 正在启动
	HP_SS_STARTED	= 1,	// 已经启动
	HP_SS_STOPING	= 2,	// 正在停止
	HP_SS_STOPED	= 3,	// 已经停止
};

/************************************************************************
名称：Socket 操作类型
描述：应用程序的 OnErrror() 事件中通过该参数标识是哪种操作导致的错误
************************************************************************/
enum En_HP_SocketOperation
{
	HP_SO_UNKNOWN	= 0,	// Unknown
	HP_SO_ACCEPT	= 1,	// Acccept
	HP_SO_CONNECT	= 2,	// Connect
	HP_SO_SEND		= 3,	// Send
	HP_SO_RECEIVE	= 4,	// Receive
};

/************************************************************************
名称：事件通知处理结果
描述：事件通知的返回值，不同的返回值会影响通信组件的后续行为
************************************************************************/
enum En_HP_HandleResult
{
	HP_HR_OK		= 0,	// 成功
	HP_HR_IGNORE	= 1,	// 忽略
	HP_HR_ERROR		= 2,	// 错误
};

/************************************************************************
名称：数据抓取结果
描述：数据抓取操作的返回值
************************************************************************/
enum En_HP_FetchResult
{
	HP_FR_OK				= 0,	// 成功
	HP_FR_LENGTH_TOO_LONG	= 1,	// 抓取长度过大
	HP_FR_DATA_NOT_FOUND	= 2,	// 找不到 ConnID 对应的数据
};

/************************************************************************
名称：操作结果代码
描述：组件 Start() / Stop() 方法执行失败时，可通过 GetLastError() 获取错误代码
************************************************************************/
enum En_HP_SocketError
{
	HP_SE_OK					= NO_ERROR,	// 成功
	HP_SE_ILLEGAL_STATE			= 1,		// 当前状态不允许操作
	HP_SE_INVALID_PARAM			= 2,		// 非法参数
	HP_SE_SOCKET_CREATE			= 3,		// 创建 SOCKET 失败
	HP_SE_SOCKET_BIND			= 4,		// 绑定 SOCKET 失败
	HP_SE_SOCKET_PREPARE		= 5,		// 设置 SOCKET 失败
	HP_SE_SOCKET_LISTEN			= 6,		// 监听 SOCKET 失败
	HP_SE_CP_CREATE				= 7,		// 创建完成端口失败
	HP_SE_WORKER_THREAD_CREATE	= 8,		// 创建工作线程失败
	HP_SE_DETECT_THREAD_CREATE	= 9,		// 创建监测线程失败
	HP_SE_SOCKE_ATTACH_TO_CP	= 10,		// 绑定完成端口失败
	HP_SE_CONNECT_SERVER		= 11,		// 连接服务器失败
	HP_SE_NETWORK				= 12,		// 网络错误
	HP_SE_DATA_PROC				= 13,		// 数据处理错误
	HP_SE_DATA_SEND				= 14,		// 数据发送失败
};

/****************************************************/
/************** HPSocket4C.dll 回调函数 **************/

/* 公共回调函数 */
typedef En_HP_HandleResult (__stdcall *HP_FN_OnSend)			(HP_CONNID dwConnID, const BYTE* pData, int iLength);
typedef En_HP_HandleResult (__stdcall *HP_FN_OnReceive)			(HP_CONNID dwConnID, const BYTE* pData, int iLength);
typedef En_HP_HandleResult (__stdcall *HP_FN_OnPullReceive)		(HP_CONNID dwConnID, int iLength);
typedef En_HP_HandleResult (__stdcall *HP_FN_OnClose)			(HP_CONNID dwConnID);
typedef En_HP_HandleResult (__stdcall *HP_FN_OnError)			(HP_CONNID dwConnID, En_HP_SocketOperation enOperation, int iErrorCode);

/* 服务端回调函数 */
typedef En_HP_HandleResult (__stdcall *HP_FN_OnPrepareListen)	(UINT_PTR soListen);
// 如果为 TCP 连接，pClient为 SOCKET 句柄；如果为 UDP 连接，pClient为 SOCKADDR_IN 指针；
typedef En_HP_HandleResult (__stdcall *HP_FN_OnAccept)			(HP_CONNID dwConnID, UINT_PTR pClient);
typedef En_HP_HandleResult (__stdcall *HP_FN_OnServerShutdown)	();

/* 客户端和 Agent 回调函数 */
typedef En_HP_HandleResult (__stdcall *HP_FN_OnPrepareConnect)	(HP_CONNID dwConnID, UINT_PTR socket);
typedef En_HP_HandleResult (__stdcall *HP_FN_OnConnect)			(HP_CONNID dwConnID);

/* Agent 回调函数 */
typedef En_HP_HandleResult (__stdcall *HP_FN_OnAgentShutdown)	();

/****************************************************/
/************** HPSocket4C.dll 导出函数 **************/

// 创建 HP_TcpServer 对象
HPSOCKET_API HP_TcpServer __stdcall Create_HP_TcpServer(HP_TcpServerListener pListener);
// 创建 HP_TcpClient 对象
HPSOCKET_API HP_TcpClient __stdcall Create_HP_TcpClient(HP_TcpClientListener pListener);
// 创建 HP_TcpAgent 对象
HPSOCKET_API HP_TcpAgent __stdcall Create_HP_TcpAgent(HP_TcpAgentListener pListener);
// 创建 HP_TcpPullServer 对象
HPSOCKET_API HP_TcpPullServer __stdcall Create_HP_TcpPullServer(HP_TcpPullServerListener pListener);
// 创建 HP_TcpPullClient 对象
HPSOCKET_API HP_TcpPullClient __stdcall Create_HP_TcpPullClient(HP_TcpPullClientListener pListener);
// 创建 HP_TcpPullAgent 对象
HPSOCKET_API HP_TcpPullAgent __stdcall Create_HP_TcpPullAgent(HP_TcpPullAgentListener pListener);
// 创建 HP_UdpServer 对象
HPSOCKET_API HP_UdpServer __stdcall Create_HP_UdpServer(HP_UdpServerListener pListener);
// 创建 HP_UdpClient 对象
HPSOCKET_API HP_UdpClient __stdcall Create_HP_UdpClient(HP_UdpClientListener pListener);

// 销毁 HP_TcpServer 对象
HPSOCKET_API void __stdcall Destroy_HP_TcpServer(HP_TcpServer pServer);
// 销毁 HP_TcpClient 对象
HPSOCKET_API void __stdcall Destroy_HP_TcpClient(HP_TcpClient pClient);
// 销毁 HP_TcpAgent 对象
HPSOCKET_API void __stdcall Destroy_HP_TcpAgent(HP_TcpAgent pAgent);
// 销毁 HP_TcpPullServer 对象
HPSOCKET_API void __stdcall Destroy_HP_TcpPullServer(HP_TcpPullServer pServer);
// 销毁 HP_TcpPullClient 对象
HPSOCKET_API void __stdcall Destroy_HP_TcpPullClient(HP_TcpPullClient pClient);
// 销毁 HP_TcpPullAgent 对象
HPSOCKET_API void __stdcall Destroy_HP_TcpPullAgent(HP_TcpPullAgent pAgent);
// 销毁 HP_UdpServer 对象
HPSOCKET_API void __stdcall Destroy_HP_UdpServer(HP_UdpServer pServer);
// 销毁 HP_UdpClient 对象
HPSOCKET_API void __stdcall Destroy_HP_UdpClient(HP_UdpClient pClient);

// 创建 HP_TcpServerListener 对象
HPSOCKET_API HP_TcpServerListener __stdcall Create_HP_TcpServerListener();
// 创建 HP_TcpClientListener 对象
HPSOCKET_API HP_TcpClientListener __stdcall Create_HP_TcpClientListener();
// 创建 HP_TcpAgentListener 对象
HPSOCKET_API HP_TcpAgentListener __stdcall Create_HP_TcpAgentListener();
// 创建 HP_TcpPullServerListener 对象
HPSOCKET_API HP_TcpPullServerListener __stdcall Create_HP_TcpPullServerListener();
// 创建 HP_TcpPullClientListener 对象
HPSOCKET_API HP_TcpPullClientListener __stdcall Create_HP_TcpPullClientListener();
// 创建 HP_TcpPullAgentListener 对象
HPSOCKET_API HP_TcpPullAgentListener __stdcall Create_HP_TcpPullAgentListener();
// 创建 HP_UdpServerListener 对象
HPSOCKET_API HP_UdpServerListener __stdcall Create_HP_UdpServerListener();
// 创建 HP_UdpClientListener 对象
HPSOCKET_API HP_UdpClientListener __stdcall Create_HP_UdpClientListener();

// 销毁 HP_TcpServerListener 对象
HPSOCKET_API void __stdcall Destroy_HP_TcpServerListener(HP_TcpServerListener pListener);
// 销毁 HP_TcpClientListener 对象
HPSOCKET_API void __stdcall Destroy_HP_TcpClientListener(HP_TcpClientListener pListener);
// 销毁 HP_TcpAgentListener 对象
HPSOCKET_API void __stdcall Destroy_HP_TcpAgentListener(HP_TcpAgentListener pListener);
// 销毁 HP_TcpPullServerListener 对象
HPSOCKET_API void __stdcall Destroy_HP_TcpPullServerListener(HP_TcpPullServerListener pListener);
// 销毁 HP_TcpPullClientListener 对象
HPSOCKET_API void __stdcall Destroy_HP_TcpPullClientListener(HP_TcpPullClientListener pListener);
// 销毁 HP_TcpPullAgentListener 对象
HPSOCKET_API void __stdcall Destroy_HP_TcpPullAgentListener(HP_TcpPullAgentListener pListener);
// 销毁 HP_UdpServerListener 对象
HPSOCKET_API void __stdcall Destroy_HP_UdpServerListener(HP_UdpServerListener pListener);
// 销毁 HP_UdpClientListener 对象
HPSOCKET_API void __stdcall Destroy_HP_UdpClientListener(HP_UdpClientListener pListener);

/**********************************************************************************/
/***************************** Server 回调函数设置方法 *****************************/

HPSOCKET_API void __stdcall HP_Set_FN_Server_OnPrepareListen(HP_ServerListener pListener	, HP_FN_OnPrepareListen fn);
HPSOCKET_API void __stdcall HP_Set_FN_Server_OnAccept(HP_ServerListener pListener			, HP_FN_OnAccept fn);
HPSOCKET_API void __stdcall HP_Set_FN_Server_OnSend(HP_ServerListener pListener				, HP_FN_OnSend fn);
HPSOCKET_API void __stdcall HP_Set_FN_Server_OnReceive(HP_ServerListener pListener			, HP_FN_OnReceive fn);
HPSOCKET_API void __stdcall HP_Set_FN_Server_OnPullReceive(HP_ServerListener pListener		, HP_FN_OnPullReceive fn);
HPSOCKET_API void __stdcall HP_Set_FN_Server_OnClose(HP_ServerListener pListener			, HP_FN_OnClose fn);
HPSOCKET_API void __stdcall HP_Set_FN_Server_OnError(HP_ServerListener pListener			, HP_FN_OnError fn);
HPSOCKET_API void __stdcall HP_Set_FN_Server_OnServerShutdown(HP_ServerListener pListener	, HP_FN_OnServerShutdown fn);

/**********************************************************************************/
/***************************** Client 回调函数设置方法 *****************************/

HPSOCKET_API void __stdcall HP_Set_FN_Client_OnPrepareConnect(HP_ClientListener pListener	, HP_FN_OnPrepareConnect fn);
HPSOCKET_API void __stdcall HP_Set_FN_Client_OnConnect(HP_ClientListener pListener			, HP_FN_OnConnect fn);
HPSOCKET_API void __stdcall HP_Set_FN_Client_OnSend(HP_ClientListener pListener				, HP_FN_OnSend fn);
HPSOCKET_API void __stdcall HP_Set_FN_Client_OnReceive(HP_ClientListener pListener			, HP_FN_OnReceive fn);
HPSOCKET_API void __stdcall HP_Set_FN_Client_OnPullReceive(HP_ClientListener pListener		, HP_FN_OnPullReceive fn);
HPSOCKET_API void __stdcall HP_Set_FN_Client_OnClose(HP_ClientListener pListener			, HP_FN_OnClose fn);
HPSOCKET_API void __stdcall HP_Set_FN_Client_OnError(HP_ClientListener pListener			, HP_FN_OnError fn);

/**********************************************************************************/
/****************************** Agent 回调函数设置方法 *****************************/

HPSOCKET_API void __stdcall HP_Set_FN_Agent_OnPrepareConnect(HP_AgentListener pListener		, HP_FN_OnPrepareConnect fn);
HPSOCKET_API void __stdcall HP_Set_FN_Agent_OnConnect(HP_AgentListener pListener			, HP_FN_OnConnect fn);
HPSOCKET_API void __stdcall HP_Set_FN_Agent_OnSend(HP_AgentListener pListener				, HP_FN_OnSend fn);
HPSOCKET_API void __stdcall HP_Set_FN_Agent_OnReceive(HP_AgentListener pListener			, HP_FN_OnReceive fn);
HPSOCKET_API void __stdcall HP_Set_FN_Agent_OnPullReceive(HP_AgentListener pListener		, HP_FN_OnPullReceive fn);
HPSOCKET_API void __stdcall HP_Set_FN_Agent_OnClose(HP_AgentListener pListener				, HP_FN_OnClose fn);
HPSOCKET_API void __stdcall HP_Set_FN_Agent_OnError(HP_AgentListener pListener				, HP_FN_OnError fn);
HPSOCKET_API void __stdcall HP_Set_FN_Agent_OnAgentShutdown(HP_AgentListener pListener		, HP_FN_OnAgentShutdown fn);

/**************************************************************************/
/***************************** Server 操作方法 *****************************/

/*
* 名称：启动通信组件
* 描述：启动服务端通信组件，启动完成后可开始接收客户端连接并收发数据
*		
* 参数：		pszBindAddress	-- 监听地址
*			usPort			-- 监听端口
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败，可通过 GetLastError() 获取错误代码
*/
HPSOCKET_API BOOL __stdcall HP_Server_Start(HP_Server pServer, LPCTSTR pszBindAddress, USHORT usPort);

/*
* 名称：关闭通信组件
* 描述：关闭服务端通信组件，关闭完成后断开所有客户端连接并释放所有资源
*		
* 参数：	
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败，可通过 GetLastError() 获取错误代码
*/
HPSOCKET_API BOOL __stdcall HP_Server_Stop(HP_Server pServer);

/*
* 名称：发送数据
* 描述：用户通过该方法向指定客户端发送数据
*		
* 参数：		dwConnID	-- 连接 ID
*			pBuffer		-- 发送数据缓冲区
*			iLength		-- 发送数据长度
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败，可通过函数 SYS_GetLastError() 获取 Windows 错误代码
*/
HPSOCKET_API BOOL __stdcall HP_Server_Send(HP_Server pServer, HP_CONNID dwConnID, const BYTE* pBuffer, int iLength);

/*
* 名称：断开连接
* 描述：断开与某个客户端的连接
*		
* 参数：		dwConnID	-- 连接 ID
*			bForce		-- 是否强制断开连接
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败
*/
HPSOCKET_API BOOL __stdcall HP_Server_Disconnect(HP_Server pServer, HP_CONNID dwConnID, BOOL bForce);

/*
* 名称：断开超时连接
* 描述：断开超过指定时长的连接
*		
* 参数：		dwPeriod	-- 时长（毫秒）
*			bForce		-- 是否强制断开连接
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败
*/
HPSOCKET_API BOOL __stdcall HP_Server_DisconnectLongConnections(HP_Server pServer, DWORD dwPeriod, BOOL bForce);

/******************************************************************************/
/***************************** Server 属性访问方法 *****************************/

/*
* 名称：设置连接的附加数据
* 描述：是否为连接绑定附加数据或者绑定什么样的数据，均由应用程序只身决定
*		
* 参数：		dwConnID	-- 连接 ID
*			pv			-- 数据
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败（无效的连接 ID）
*/
HPSOCKET_API BOOL __stdcall HP_Server_SetConnectionExtra(HP_Server pServer, HP_CONNID dwConnID, PVOID pExtra);

/*
* 名称：获取连接的附加数据
* 描述：是否为连接绑定附加数据或者绑定什么样的数据，均由应用程序只身决定
*		
* 参数：		dwConnID	-- 连接 ID
*			ppv			-- 数据指针
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败（无效的连接 ID）
*/
HPSOCKET_API BOOL __stdcall HP_Server_GetConnectionExtra(HP_Server pServer, HP_CONNID dwConnID, PVOID* ppExtra);

/* 检查通信组件是否已启动 */
HPSOCKET_API BOOL __stdcall HP_Server_HasStarted(HP_Server pServer);
/* 查看通信组件当前状态 */
HPSOCKET_API En_HP_ServiceState __stdcall HP_Server_GetState(HP_Server pServer);
/* 获取最近一次失败操作的错误代码 */
HPSOCKET_API En_HP_SocketError __stdcall HP_Server_GetLastError(HP_Server pServer);
/* 获取最近一次失败操作的错误描述 */
HPSOCKET_API LPCTSTR __stdcall HP_Server_GetLastErrorDesc(HP_Server pServer);
/* 获取客户端连接数 */
HPSOCKET_API DWORD __stdcall HP_Server_GetConnectionCount(HP_Server pServer);
/* 获取某个客户端连接时长（毫秒） */
HPSOCKET_API BOOL __stdcall HP_Server_GetConnectPeriod(HP_Server pServer, HP_CONNID dwConnID, DWORD* pdwPeriod);
/* 获取监听 Socket 的地址信息 */
HPSOCKET_API BOOL __stdcall HP_Server_GetListenAddress(HP_Server pServer, LPTSTR lpszAddress, int* piAddressLen, USHORT* pusPort);
/* 获取某个连接的远程地址信息 */
HPSOCKET_API BOOL __stdcall HP_Server_GetRemoteAddress(HP_Server pServer, HP_CONNID dwConnID, LPTSTR lpszAddress, int* piAddressLen, USHORT* pusPort);

/* 设置 Socket 缓存对象锁定时间（毫秒，在锁定期间该 Socket 缓存对象不能被获取使用） */
HPSOCKET_API void __stdcall HP_Server_SetFreeSocketObjLockTime(HP_Server pServer, DWORD dwFreeSocketObjLockTime);
/* 设置 Socket 缓存池大小（通常设置为平均并发连接数量的 1/3 - 1/2） */
HPSOCKET_API void __stdcall HP_Server_SetFreeSocketObjPool(HP_Server pServer, DWORD dwFreeSocketObjPool);
/* 设置内存块缓存池大小（通常设置为 Socket 缓存池大小的 2 - 3 倍） */
HPSOCKET_API void __stdcall HP_Server_SetFreeBufferObjPool(HP_Server pServer, DWORD dwFreeBufferObjPool);
/* 设置 Socket 缓存池回收阀值（通常设置为 Socket 缓存池大小的 3 倍） */
HPSOCKET_API void __stdcall HP_Server_SetFreeSocketObjHold(HP_Server pServer, DWORD dwFreeSocketObjHold);
/* 设置内存块缓存池回收阀值（通常设置为内存块缓存池大小的 3 倍） */
HPSOCKET_API void __stdcall HP_Server_SetFreeBufferObjHold(HP_Server pServer, DWORD dwFreeBufferObjHold);
/* 设置工作线程数量（通常设置为 2 * CPU + 2） */
HPSOCKET_API void __stdcall HP_Server_SetWorkerThreadCount(HP_Server pServer, DWORD dwWorkerThreadCount);
/* 设置关闭服务前等待连接关闭的最长时限（毫秒，0 则不等待） */
HPSOCKET_API void __stdcall HP_Server_SetMaxShutdownWaitTime(HP_Server pServer, DWORD dwMaxShutdownWaitTime);

/* 获取 Socket 缓存对象锁定时间 */
HPSOCKET_API DWORD __stdcall HP_Server_GetFreeSocketObjLockTime(HP_Server pServer);
/* 获取 Socket 缓存池大小 */
HPSOCKET_API DWORD __stdcall HP_Server_GetFreeSocketObjPool(HP_Server pServer);
/* 获取内存块缓存池大小 */
HPSOCKET_API DWORD __stdcall HP_Server_GetFreeBufferObjPool(HP_Server pServer);
/* 获取 Socket 缓存池回收阀值 */
HPSOCKET_API DWORD __stdcall HP_Server_GetFreeSocketObjHold(HP_Server pServer);
/* 获取内存块缓存池回收阀值 */
HPSOCKET_API DWORD __stdcall HP_Server_GetFreeBufferObjHold(HP_Server pServer);
/* 获取工作线程数量 */
HPSOCKET_API DWORD __stdcall HP_Server_GetWorkerThreadCount(HP_Server pServer);
/* 获取关闭服务前等待连接关闭的最长时限 */
HPSOCKET_API DWORD __stdcall HP_Server_GetMaxShutdownWaitTime(HP_Server pServer);

/**********************************************************************************/
/***************************** TCP Server 属性访问方法 *****************************/

/* 设置 Accept 预投递 Socket 数量（通常设置为工作线程数的 1 - 2 倍） */
HPSOCKET_API void __stdcall HP_TcpServer_SetAcceptSocketCount(HP_TcpServer pServer, DWORD dwAcceptSocketCount);
/* 设置通信数据缓冲区大小（根据平均通信数据包大小调整设置，通常设置为 1024 的倍数） */
HPSOCKET_API void __stdcall HP_TcpServer_SetSocketBufferSize(HP_TcpServer pServer, DWORD dwSocketBufferSize);
/* 设置监听 Socket 的等候队列大小（根据并发连接数量调整设置） */
HPSOCKET_API void __stdcall HP_TcpServer_SetSocketListenQueue(HP_TcpServer pServer, DWORD dwSocketListenQueue);
/* 设置心跳包间隔（毫秒，0 则不发送心跳包） */
HPSOCKET_API void __stdcall HP_TcpServer_SetKeepAliveTime(HP_TcpServer pServer, DWORD dwKeepAliveTime);
/* 设置心跳确认包检测间隔（毫秒，0 不发送心跳包，如果超过若干次 [默认：WinXP 5 次, Win7 10 次] 检测不到心跳确认包则认为已断线） */
HPSOCKET_API void __stdcall HP_TcpServer_SetKeepAliveInterval(HP_TcpServer pServer, DWORD dwKeepAliveInterval);

/* 获取 Accept 预投递 Socket 数量 */
HPSOCKET_API DWORD __stdcall HP_TcpServer_GetAcceptSocketCount(HP_TcpServer pServer);
/* 获取通信数据缓冲区大小 */
HPSOCKET_API DWORD __stdcall HP_TcpServer_GetSocketBufferSize(HP_TcpServer pServer);
/* 获取监听 Socket 的等候队列大小 */
HPSOCKET_API DWORD __stdcall HP_TcpServer_GetSocketListenQueue(HP_TcpServer pServer);
/* 获取心跳检查次数 */
HPSOCKET_API DWORD __stdcall HP_TcpServer_GetKeepAliveTime(HP_TcpServer pServer);
/* 获取心跳检查间隔 */
HPSOCKET_API DWORD __stdcall HP_TcpServer_GetKeepAliveInterval(HP_TcpServer pServer);

/**********************************************************************************/
/***************************** UDP Server 属性访问方法 *****************************/

/* 设置数据报文最大长度（建议在局域网环境下不超过 1472 字节，在广域网环境下不超过 548 字节） */
HPSOCKET_API void __stdcall HP_UdpServer_SetMaxDatagramSize(HP_UdpServer pServer, DWORD dwMaxDatagramSize);
/* 获取数据报文最大长度 */
HPSOCKET_API DWORD __stdcall HP_UdpServer_GetMaxDatagramSize(HP_UdpServer pServer);

/* 设置监测包尝试次数（0 则不发送监测跳包，如果超过最大尝试次数则认为已断线） */
HPSOCKET_API void __stdcall HP_UdpServer_SetDetectAttempts(HP_UdpServer pServer, DWORD dwDetectAttempts);
/* 设置监测包发送间隔（秒，0 不发送监测包） */
HPSOCKET_API void __stdcall HP_UdpServer_SetDetectInterval(HP_UdpServer pServer, DWORD dwDetectInterval);
/* 获取心跳检查次数 */
HPSOCKET_API DWORD __stdcall HP_UdpServer_GetDetectAttempts(HP_UdpServer pServer);
/* 获取心跳检查间隔 */
HPSOCKET_API DWORD __stdcall HP_UdpServer_GetDetectInterval(HP_UdpServer pServer);

/******************************************************************************/
/***************************** Client 组件操作方法 *****************************/

/*
* 名称：启动通信组件
* 描述：启动客户端通信组件并连接服务端，启动完成后可开始收发数据
*		
* 参数：		pszRemoteAddress	-- 服务端地址
*			usPort				-- 服务端端口
*			bAsyncConnect		-- 是否采用异步 Connect
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败，可通过 GetLastError() 获取错误代码
*/
HPSOCKET_API BOOL __stdcall HP_Client_Start(HP_Client pClient, LPCTSTR pszRemoteAddress, USHORT usPort, BOOL bAsyncConnect);

/*
* 名称：关闭通信组件
* 描述：关闭客户端通信组件，关闭完成后断开与服务端的连接并释放所有资源
*		
* 参数：	
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败，可通过 GetLastError() 获取错误代码
*/
HPSOCKET_API BOOL __stdcall HP_Client_Stop(HP_Client pClient);

/*
* 名称：发送数据
* 描述：用户通过该方法向服务端发送数据
*		
* 参数：		dwConnID	-- 连接 ID（保留参数，目前该参数并未使用）
*			pBuffer		-- 发送数据缓冲区
*			iLength		-- 发送数据长度
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败，可通过函数 SYS_GetLastError() 获取 Windows 错误代码
*/
HPSOCKET_API BOOL __stdcall HP_Client_Send(HP_Client pClient, HP_CONNID dwConnID, const BYTE* pBuffer, int iLength);

/******************************************************************************/
/***************************** Client 属性访问方法 *****************************/

/* 检查通信组件是否已启动 */
HPSOCKET_API BOOL __stdcall HP_Client_HasStarted(HP_Client pClient);
/* 查看通信组件当前状态 */
HPSOCKET_API En_HP_ServiceState	__stdcall HP_Client_GetState(HP_Client pClient);
/* 获取最近一次失败操作的错误代码 */
HPSOCKET_API En_HP_SocketError	__stdcall HP_Client_GetLastError(HP_Client pClient);
/* 获取最近一次失败操作的错误描述 */
HPSOCKET_API LPCTSTR __stdcall HP_Client_GetLastErrorDesc(HP_Client pClient);
/* 获取该组件对象的连接 ID */
HPSOCKET_API HP_CONNID __stdcall HP_Client_GetConnectionID(HP_Client pClient);
/* 获取 Client Socket 的地址信息 */
HPSOCKET_API BOOL __stdcall HP_Client_GetLocalAddress(HP_Client pClient, LPTSTR lpszAddress, int* piAddressLen, USHORT* pusPort);
/* 设置内存块缓存池大小（通常设置为 -> PUSH 模型：5 - 10；PULL 模型：10 - 20 ） */
HPSOCKET_API void __stdcall HP_Client_SetFreeBufferPoolSize(HP_Client pClient, DWORD dwFreeBufferPoolSize);
/* 设置内存块缓存池回收阀值（通常设置为内存块缓存池大小的 3 倍） */
HPSOCKET_API void __stdcall HP_Client_SetFreeBufferPoolHold(HP_Client pClient, DWORD dwFreeBufferPoolHold);
/* 获取内存块缓存池大小 */
HPSOCKET_API DWORD __stdcall HP_Client_GetFreeBufferPoolSize(HP_Client pClient);
/* 获取内存块缓存池回收阀值 */
HPSOCKET_API DWORD __stdcall HP_Client_GetFreeBufferPoolHold(HP_Client pClient);

/**********************************************************************************/
/***************************** TCP Client 属性访问方法 *****************************/
	
/* 设置通信数据缓冲区大小（根据平均通信数据包大小调整设置，通常设置为：(N * 1024) - sizeof(TBufferObj)） */
HPSOCKET_API void __stdcall HP_TcpClient_SetSocketBufferSize(HP_TcpClient pClient, DWORD dwSocketBufferSize);
/* 设置心跳包间隔（毫秒，0 则不发送心跳包） */
HPSOCKET_API void __stdcall HP_TcpClient_SetKeepAliveTime(HP_TcpClient pClient, DWORD dwKeepAliveTime);
/* 设置心跳确认包检测间隔（毫秒，0 不发送心跳包，如果超过若干次 [默认：WinXP 5 次, Win7 10 次] 检测不到心跳确认包则认为已断线） */
HPSOCKET_API void __stdcall HP_TcpClient_SetKeepAliveInterval(HP_TcpClient pClient, DWORD dwKeepAliveInterval);

/* 获取通信数据缓冲区大小 */
HPSOCKET_API DWORD __stdcall HP_TcpClient_GetSocketBufferSize(HP_TcpClient pClient);
/* 获取心跳检查次数 */
HPSOCKET_API DWORD __stdcall HP_TcpClient_GetKeepAliveTime(HP_TcpClient pClient);
/* 获取心跳检查间隔 */
HPSOCKET_API DWORD __stdcall HP_TcpClient_GetKeepAliveInterval(HP_TcpClient pClient);

/**********************************************************************************/
/***************************** UDP Client 属性访问方法 *****************************/

/* 设置数据报文最大长度（建议在局域网环境下不超过 1472 字节，在广域网环境下不超过 548 字节） */
HPSOCKET_API void __stdcall HP_UdpClient_SetMaxDatagramSize(HP_UdpClient pClient, DWORD dwMaxDatagramSize);
/* 获取数据报文最大长度 */
HPSOCKET_API DWORD __stdcall HP_UdpClient_GetMaxDatagramSize(HP_UdpClient pClient);

/* 设置监测包尝试次数（0 则不发送监测跳包，如果超过最大尝试次数则认为已断线） */
HPSOCKET_API void __stdcall HP_UdpClient_SetDetectAttempts(HP_UdpClient pClient, DWORD dwDetectAttempts);
/* 设置监测包发送间隔（秒，0 不发送监测包） */
HPSOCKET_API void __stdcall HP_UdpClient_SetDetectInterval(HP_UdpClient pClient, DWORD dwDetectInterval);
/* 获取心跳检查次数 */
HPSOCKET_API DWORD __stdcall HP_UdpClient_GetDetectAttempts(HP_UdpClient pClient);
/* 获取心跳检查间隔 */
HPSOCKET_API DWORD __stdcall HP_UdpClient_GetDetectInterval(HP_UdpClient pClient);

/**************************************************************************/
/***************************** Agent 操作方法 *****************************/

/*
* 名称：启动通信组件
* 描述：启动通信代理组件，启动完成后可开始连接远程服务器
*		
* 参数：		pszBindAddress	-- 监听地址
*			bAsyncConnect	-- 是否采用异步 Connect
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败，可通过 GetLastError() 获取错误代码
*/
HPSOCKET_API BOOL __stdcall HP_Agent_Start(HP_Agent pAgent, LPCTSTR pszBindAddress, BOOL bAsyncConnect);

/*
* 名称：关闭通信组件
* 描述：关闭通信组件，关闭完成后断开所有连接并释放所有资源
*		
* 参数：	
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败，可通过 GetLastError() 获取错误代码
*/
HPSOCKET_API BOOL __stdcall HP_Agent_Stop(HP_Agent pAgent);

/*
* 名称：连接服务器
* 描述：连接服务器，连接成功后 IAgentListener 会接收到 OnConnect() 事件
*		
* 参数：		pszRemoteAddress	-- 服务端地址
*			usPort				-- 服务端端口
*			pdwConnID			-- 连接 ID（默认：nullptr，不获取连接 ID）
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败，可通过函数 SYS_GetLastError() 获取 Windows 错误代码
*/
HPSOCKET_API BOOL __stdcall HP_Agent_Connect(HP_Agent pAgent, LPCTSTR pszRemoteAddress, USHORT usPort, HP_CONNID* pdwConnID);

/*
* 名称：发送数据
* 描述：用户通过该方法向指定连接发送数据
*		
* 参数：		dwConnID	-- 连接 ID
*			pBuffer		-- 发送数据缓冲区
*			iLength		-- 发送数据长度
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败，可通过函数 SYS_GetLastError() 获取 Windows 错误代码
*/
HPSOCKET_API BOOL __stdcall HP_Agent_Send(HP_Agent pAgent, HP_CONNID dwConnID, const BYTE* pBuffer, int iLength);

/*
* 名称：断开连接
* 描述：断开某个连接
*		
* 参数：		dwConnID	-- 连接 ID
*			bForce		-- 是否强制断开连接
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败
*/
HPSOCKET_API BOOL __stdcall HP_Agent_Disconnect(HP_Agent pAgent, HP_CONNID dwConnID, BOOL bForce);

/*
* 名称：断开超时连接
* 描述：断开超过指定时长的连接
*		
* 参数：		dwPeriod	-- 时长（毫秒）
*			bForce		-- 是否强制断开连接
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败
*/
HPSOCKET_API BOOL __stdcall HP_Agent_DisconnectLongConnections(HP_Agent pAgent, DWORD dwPeriod, BOOL bForce);

/******************************************************************************/
/***************************** Agent 属性访问方法 *****************************/

/*
* 名称：设置连接的附加数据
* 描述：是否为连接绑定附加数据或者绑定什么样的数据，均由应用程序只身决定
*		
* 参数：		dwConnID	-- 连接 ID
*			pv			-- 数据
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败（无效的连接 ID）
*/
HPSOCKET_API BOOL __stdcall HP_Agent_SetConnectionExtra(HP_Agent pAgent, HP_CONNID dwConnID, PVOID pExtra);

/*
* 名称：获取连接的附加数据
* 描述：是否为连接绑定附加数据或者绑定什么样的数据，均由应用程序只身决定
*		
* 参数：		dwConnID	-- 连接 ID
*			ppv			-- 数据指针
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败（无效的连接 ID）
*/
HPSOCKET_API BOOL __stdcall HP_Agent_GetConnectionExtra(HP_Agent pAgent, HP_CONNID dwConnID, PVOID* ppExtra);

/* 检查通信组件是否已启动 */
HPSOCKET_API BOOL __stdcall HP_Agent_HasStarted(HP_Agent pAgent);
/* 查看通信组件当前状态 */
HPSOCKET_API En_HP_ServiceState __stdcall HP_Agent_GetState(HP_Agent pAgent);
/* 获取连接数 */
HPSOCKET_API DWORD __stdcall HP_Agent_GetConnectionCount(HP_Agent pAgent);
/* 获取某个连接时长（毫秒） */
HPSOCKET_API BOOL __stdcall HP_Agent_GetConnectPeriod(HP_Agent pAgent, HP_CONNID dwConnID, DWORD* pdwPeriod);
/* 获取某个连接的本地地址信息 */
HPSOCKET_API BOOL __stdcall HP_Agent_GetLocalAddress(HP_Agent pAgent, HP_CONNID dwConnID, LPTSTR lpszAddress, int* piAddressLen, USHORT* pusPort);
/* 获取某个连接的远程地址信息 */
HPSOCKET_API BOOL __stdcall HP_Agent_GetRemoteAddress(HP_Agent pAgent, HP_CONNID dwConnID, LPTSTR lpszAddress, int* piAddressLen, USHORT* pusPort);
/* 获取最近一次失败操作的错误代码 */
HPSOCKET_API En_HP_SocketError __stdcall HP_Agent_GetLastError(HP_Agent pAgent);
/* 获取最近一次失败操作的错误描述 */
HPSOCKET_API LPCTSTR __stdcall HP_Agent_GetLastErrorDesc(HP_Agent pAgent);

/* 设置 Socket 缓存对象锁定时间（毫秒，在锁定期间该 Socket 缓存对象不能被获取使用） */
HPSOCKET_API void __stdcall HP_Agent_SetFreeSocketObjLockTime(HP_Agent pAgent, DWORD dwFreeSocketObjLockTime);
/* 设置 Socket 缓存池大小（通常设置为平均并发连接数量的 1/3 - 1/2） */
HPSOCKET_API void __stdcall HP_Agent_SetFreeSocketObjPool(HP_Agent pAgent, DWORD dwFreeSocketObjPool);
/* 设置内存块缓存池大小（通常设置为 Socket 缓存池大小的 2 - 3 倍） */
HPSOCKET_API void __stdcall HP_Agent_SetFreeBufferObjPool(HP_Agent pAgent, DWORD dwFreeBufferObjPool);
/* 设置 Socket 缓存池回收阀值（通常设置为 Socket 缓存池大小的 3 倍） */
HPSOCKET_API void __stdcall HP_Agent_SetFreeSocketObjHold(HP_Agent pAgent, DWORD dwFreeSocketObjHold);
/* 设置内存块缓存池回收阀值（通常设置为内存块缓存池大小的 3 倍） */
HPSOCKET_API void __stdcall HP_Agent_SetFreeBufferObjHold(HP_Agent pAgent, DWORD dwFreeBufferObjHold);
/* 设置工作线程数量（通常设置为 2 * CPU + 2） */
HPSOCKET_API void __stdcall HP_Agent_SetWorkerThreadCount(HP_Agent pAgent, DWORD dwWorkerThreadCount);
/* 设置关闭组件前等待连接关闭的最长时限（毫秒，0 则不等待） */
HPSOCKET_API void __stdcall HP_Agent_SetMaxShutdownWaitTime(HP_Agent pAgent, DWORD dwMaxShutdownWaitTime);

/* 获取 Socket 缓存对象锁定时间 */
HPSOCKET_API DWORD __stdcall HP_Agent_GetFreeSocketObjLockTime(HP_Agent pAgent);
/* 获取 Socket 缓存池大小 */
HPSOCKET_API DWORD __stdcall HP_Agent_GetFreeSocketObjPool(HP_Agent pAgent);
/* 获取内存块缓存池大小 */
HPSOCKET_API DWORD __stdcall HP_Agent_GetFreeBufferObjPool(HP_Agent pAgent);
/* 获取 Socket 缓存池回收阀值 */
HPSOCKET_API DWORD __stdcall HP_Agent_GetFreeSocketObjHold(HP_Agent pAgent);
/* 获取内存块缓存池回收阀值 */
HPSOCKET_API DWORD __stdcall HP_Agent_GetFreeBufferObjHold(HP_Agent pAgent);
/* 获取工作线程数量 */
HPSOCKET_API DWORD __stdcall HP_Agent_GetWorkerThreadCount(HP_Agent pAgent);
/* 获取关闭组件前等待连接关闭的最长时限 */
HPSOCKET_API DWORD __stdcall HP_Agent_GetMaxShutdownWaitTime(HP_Agent pAgent);

/**********************************************************************************/
/***************************** TCP Agent 属性访问方法 *****************************/

/* 设置是否启用地址重用机制（默认：不启用） */
HPSOCKET_API void __stdcall HP_TcpAgent_SetReuseAddress(HP_TcpAgent pAgent, BOOL bReuseAddress);
/* 检测是否启用地址重用机制 */
HPSOCKET_API BOOL __stdcall HP_TcpAgent_IsReuseAddress(HP_TcpAgent pAgent);

/* 设置通信数据缓冲区大小（根据平均通信数据包大小调整设置，通常设置为 1024 的倍数） */
HPSOCKET_API void __stdcall HP_TcpAgent_SetSocketBufferSize(HP_TcpAgent pAgent, DWORD dwSocketBufferSize);
/* 设置心跳包间隔（毫秒，0 则不发送心跳包） */
HPSOCKET_API void __stdcall HP_TcpAgent_SetKeepAliveTime(HP_TcpAgent pAgent, DWORD dwKeepAliveTime);
/* 设置心跳确认包检测间隔（毫秒，0 不发送心跳包，如果超过若干次 [默认：WinXP 5 次, Win7 10 次] 检测不到心跳确认包则认为已断线） */
HPSOCKET_API void __stdcall HP_TcpAgent_SetKeepAliveInterval(HP_TcpAgent pAgent, DWORD dwKeepAliveInterval);

/* 获取通信数据缓冲区大小 */
HPSOCKET_API DWORD __stdcall HP_TcpAgent_GetSocketBufferSize(HP_TcpAgent pAgent);
/* 获取心跳检查次数 */
HPSOCKET_API DWORD __stdcall HP_TcpAgent_GetKeepAliveTime(HP_TcpAgent pAgent);
/* 获取心跳检查间隔 */
HPSOCKET_API DWORD __stdcall HP_TcpAgent_GetKeepAliveInterval(HP_TcpAgent pAgent);

/***************************************************************************************/
/***************************** TCP Pull Server 组件操作方法 *****************************/

/*
* 名称：抓取数据
* 描述：用户通过该方法从 Socket 组件中抓取数据
*		
* 参数：		dwConnID	-- 连接 ID
*			pBuffer		-- 数据抓取缓冲区
*			iLength		-- 抓取数据长度
* 返回值：	En_HP_FetchResult
*/
HPSOCKET_API En_HP_FetchResult __stdcall HP_TcpPullServer_Fetch(HP_TcpPullServer pServer, HP_CONNID dwConnID, BYTE* pBuffer, int iLength);

/***************************************************************************************/
/***************************** TCP Pull Server 属性访问方法 *****************************/

/***************************************************************************************/
/***************************** TCP Pull Client 组件操作方法 *****************************/

/*
* 名称：抓取数据
* 描述：用户通过该方法从 Socket 组件中抓取数据
*		
* 参数：		dwConnID	-- 连接 ID
*			pBuffer		-- 数据抓取缓冲区
*			iLength		-- 抓取数据长度
* 返回值：	En_HP_FetchResult
*/
HPSOCKET_API En_HP_FetchResult __stdcall HP_TcpPullClient_Fetch(HP_TcpPullClient pClient, HP_CONNID dwConnID, BYTE* pBuffer, int iLength);

/***************************************************************************************/
/***************************** TCP Pull Client 属性访问方法 *****************************/

/***************************************************************************************/
/***************************** TCP Pull Agent 组件操作方法 *****************************/

/*
* 名称：抓取数据
* 描述：用户通过该方法从 Socket 组件中抓取数据
*		
* 参数：		dwConnID	-- 连接 ID
*			pBuffer		-- 数据抓取缓冲区
*			iLength		-- 抓取数据长度
* 返回值：	En_HP_FetchResult
*/
HPSOCKET_API En_HP_FetchResult __stdcall HP_TcpPullAgent_Fetch(HP_TcpPullAgent pAgent, HP_CONNID dwConnID, BYTE* pBuffer, int iLength);

/***************************************************************************************/
/***************************** TCP Pull Agent 属性访问方法 *****************************/

/***************************************************************************************/
/*************************************** 其它方法 ***************************************/

/* 获取错误描述文本 */
HPSOCKET_API LPCTSTR __stdcall HP_GetSocketErrorDesc(En_HP_SocketError enCode);
/* 调用系统的 ::GetLastError() 方法获取系统错误代码 */
HPSOCKET_API DWORD __stdcall SYS_GetLastError();
