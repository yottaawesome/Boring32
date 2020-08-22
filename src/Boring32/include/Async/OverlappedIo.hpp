#pragma once
#include "OverlappedOp.hpp"

namespace Boring32::Async
{
	class OverlappedIo : public OverlappedOp
	{
		public:
			virtual ~OverlappedIo();
			OverlappedIo();
			OverlappedIo(
				const bool isInheritable,
				const std::wstring name
			);
			OverlappedIo(OverlappedIo&& other) noexcept;
			virtual void operator=(OverlappedIo&& other) noexcept;

		// Non-copyable
		public:
			OverlappedIo(const OverlappedIo& other) = delete;
			virtual void operator=(const OverlappedIo& other) = delete;

		public:
			std::wstring IoBuffer;
			virtual void ResizeBuffer();

		protected:
			virtual void Move(OverlappedIo& other) noexcept;
	};
}