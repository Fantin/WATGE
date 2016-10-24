#pragma once

namespace WATGE
{
	enum eWATError
	{
		eNoError,
		eGeneralError,
		eEntityDoesNotExist,
		eEntityAlreadyExists,
		eComponentDoesNotExist,
		eComponentAlreadyExists,
		eComponentClassNotSupported,
		eComponentClassAlreadySupported,
		eManagerNotSupported,
		eManagerAlreadySupported,
		eEntityManagerDoesNotExist,
		eEntityManagerAlreadyExists
	};
}
