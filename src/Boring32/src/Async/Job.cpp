#include "pch.hpp"
#include <stdexcept>
#include "include/Error/Win32Error.hpp"
#include "include/Async/Job.hpp"

namespace Boring32::Async
{
	Job::~Job()
	{ 
		Close();
	}

	void Job::Close()
	{
		m_job = nullptr;
	}

	Job::Job()
	:	m_name(L"")
	{ }

	Job::Job(const bool isInheritable)
	:	m_name(L"")
	{ 
		Create(isInheritable);
	}

	Job::Job(const bool isInheritable, std::wstring name)
	:	m_name(std::move(name))
	{
		Create(isInheritable);
	}
	
	Job::Job(const bool isInheritable, std::wstring name, const DWORD desiredAccess)
	:	m_name(std::move(name))
	{
		Open(isInheritable);
	}

	Job::Job(const Job& other)
	{
		Copy(other);
	}

	Job& Job::operator=(const Job& other)
	{
		Close();
		Copy(other);
		return *this;
	}

	void Job::Copy(const Job& other)
	{
		m_name = other.m_name;
		if (other.m_job != nullptr)
			m_job = other.m_job;
	}

	Job::Job(Job&& other) noexcept
	{
		Move(other);
	}

	Job& Job::operator=(Job&& other) noexcept
	{
		Close();
		Move(other);
		return *this;
	}

	void Job::Move(Job& other) noexcept
	{
		m_name = std::move(other.m_name);
		m_job = std::move(other.m_job);
	}

	void Job::SetInformation(JOBOBJECT_EXTENDED_LIMIT_INFORMATION& jeli)
	{
		// See https://docs.microsoft.com/en-us/windows/win32/api/winnt/ns-winnt-jobobject_basic_limit_information
		// jeli.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;

		bool bSuccess = SetInformationJobObject(
			m_job.GetHandle(),
			JobObjectExtendedLimitInformation,
			&jeli,
			sizeof(jeli)
		);
		if (bSuccess == false)
			throw Error::Win32Error("SetInformationJobObject failed", GetLastError());
	}

	void Job::AssignProcessToThisJob(const HANDLE process)
	{
		if (m_job == nullptr)
			throw std::runtime_error("Cannot assign process to job; job is not initialised");
		if (process == nullptr)
			throw std::runtime_error("Cannot assign process to job; process is null.");

		if (AssignProcessToJobObject(m_job.GetHandle(), process) == false)
			throw Error::Win32Error("Cannot assign process to job; AssignProcessToJobObject() failed.", GetLastError());
	}

	HANDLE Job::GetHandle()
	{
		return m_job.GetHandle();
	}

	const std::wstring& Job::GetName() const
	{
		return m_name;
	}
	
	bool Job::IsInheritable() const
	{
		return m_job.IsInheritable();
	}

	void Job::Open(const bool isInheritable)
	{
		m_job = OpenJobObjectW(
			JOB_OBJECT_ALL_ACCESS,
			isInheritable,
			m_name.c_str()
		);
		if (m_job == nullptr)
			throw Error::Win32Error("OpenJobObjectW() failed", GetLastError());
	}

	void Job::Create(const bool isInheritable)
	{
		m_job = CreateJobObjectW(
			nullptr, 
			m_name.size() > 0 
				? m_name.c_str() 
				: nullptr
		);
		if (m_job == nullptr)
			throw Error::Win32Error("CreateJobObjectW() failed", GetLastError());
		m_job.SetInheritability(isInheritable);
	}
}