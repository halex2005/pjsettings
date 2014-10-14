/* $Id: types.hpp 4742 2014-02-12 05:18:31Z bennylp $ */
/*
 * Copyright (C) 2013 Teluu Inc. (http://www.teluu.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#ifndef __PJSETTINGS_TYPES_HPP__
#define __PJSETTINGS_TYPES_HPP__

#ifdef _MSC_VER
#   pragma warning( disable : 4290 ) // exception spec ignored
#   pragma warning( disable : 4512 ) // can't generate assignment op
#endif

/**
 * @file pjsua2/types.hpp
 * @brief PJSUA2 Base Types
 */
#include <string>
#include <vector>

/** PJSUA2 API is inside pj namespace */
namespace pj
{

/* ************************************************************************* */

/** Signed 32bit integer. */
typedef int             pj_int32_t;

/** Unsigned 32bit integer. */
typedef unsigned int    pj_uint32_t;

/** Signed 16bit integer. */
typedef short           pj_int16_t;

/** Unsigned 16bit integer. */
typedef unsigned short  pj_uint16_t;

/** Signed 8bit integer. */
typedef signed char     pj_int8_t;

/** Unsigned 8bit integer. */
typedef unsigned char   pj_uint8_t;

/** Large unsigned integer. */
typedef size_t          pj_size_t;

/** Large signed integer. */
#if defined(PJ_WIN64) && PJ_WIN64!=0
    typedef pj_int64_t     pj_ssize_t;
#else
    typedef long           pj_ssize_t;
#endif

/** Status code. */
typedef int     pj_status_t;

/** Boolean. */
typedef int     pj_bool_t;

/** Native char type, which will be equal to wchar_t for Unicode
 * and char for ANSI. */
#if defined(PJ_NATIVE_STRING_IS_UNICODE) && PJ_NATIVE_STRING_IS_UNICODE!=0
    typedef wchar_t pj_char_t;
#else
    typedef char pj_char_t;
#endif

/** This macro creates Unicode or ANSI literal string depending whether
 *  native platform string is Unicode or ANSI. */
#if defined(PJ_NATIVE_STRING_IS_UNICODE) && PJ_NATIVE_STRING_IS_UNICODE!=0
#   define PJ_T(literal_str)    L##literal_str
#else
#   define PJ_T(literal_str)    literal_str
#endif

/** Some constants */
enum pj_constants_
{
    /** Status is OK. */
    PJ_SUCCESS=0,

    /** True value. */
    PJ_TRUE=1,

    /** False value. */
    PJ_FALSE=0
};

/* ************************************************************************* */
/*
 * Data structure types.
 */
/**
 * This type is used as replacement to legacy C string, and used throughout
 * the library. By convention, the string is NOT null terminated.
 */
struct pj_str_t
{
    /** Buffer pointer, which is by convention NOT null terminated. */
    char       *ptr;

    /** The length of the string. */
    pj_ssize_t  slen;
};


/**
 * @defgroup PJSUA2_TYPES General Data Structure
 * @ingroup PJSUA2_DS
 * @{
 */

using std::string;
using std::vector;

/** Array of strings */
typedef std::vector<std::string> StringVector;

/** Array of integers */
typedef std::vector<int> IntVector;

/**
 * Type of token, i.e. arbitrary application user data
 */
typedef void *Token;

/**
 * Socket address, encoded as string. The socket address contains host
 * and port number in "host[:port]" format. The host part may contain
 * hostname, domain name, IPv4 or IPv6 address. For IPv6 address, the
 * address will be enclosed with square brackets, e.g. "[::1]:5060".
 */
typedef string SocketAddress;

/**
 * Transport ID is an integer.
 */
typedef int TransportId;

/**
 * Transport handle, corresponds to pjsip_transport instance.
 */
typedef void *TransportHandle;

/**
 * Timer entry, corresponds to pj_timer_entry
 */
typedef void *TimerEntry;

/**
 * Generic data
 */
typedef void *GenericData;

/*
 * Forward declaration of Account and Call to be used
 * by Endpoint.
 */
class Account;
class Call;


/**
 * Constants
 */
enum
{
    /** Invalid ID, equal to PJSUA_INVALID_ID */
    INVALID_ID  = -1,

    /** Success, equal to PJ_SUCCESS */
    SUCCESS = 0
};

//////////////////////////////////////////////////////////////////////////////

/**
 * This structure contains information about an error that is thrown
 * as an exception.
 */
struct Error
{
    /** The error code. */
    pj_status_t status;

    /** The PJSUA API operation that throws the error. */
    string  title;

    /** The error message */
    string  reason;

    /** The PJSUA source file that throws the error */
    string  srcFile;

    /** The line number of PJSUA source file that throws the error */
    int     srcLine;

    /** Build error string. */
    string  info(bool multi_line=false) const;

    /** Default constructor */
    Error();

    /**
     * Construct an Error instance from the specified parameters. If
     * \a prm_reason is empty, it will be filled with the error description
     *  for the status code.
     */
    Error(pj_status_t prm_status,
          const string &prm_title,
          const string &prm_reason,
          const string &prm_src_file,
          int prm_src_line);
};


/*
 * Error utilities.
 */
#if PJSUA2_ERROR_HAS_EXTRA_INFO
#   define PJSUA2_RAISE_ERROR(status)       \
    PJSUA2_RAISE_ERROR2(status, __FUNCTION__)

#   define PJSUA2_RAISE_ERROR2(status,op)   \
    PJSUA2_RAISE_ERROR3(status, op, string())

#   define PJSUA2_RAISE_ERROR3(status,op,txt)   \
    do { \
        Error err_ = Error(status, op, txt, __FILE__, __LINE__); \
        PJ_LOG(1,(THIS_FILE, "%s", err_.info().c_str())); \
        throw err_; \
    } while (0)

#else
    /** Raise Error exception */
#   define PJSUA2_RAISE_ERROR(status)       \
    PJSUA2_RAISE_ERROR2(status, string())

/** Raise Error exception */
#   define PJSUA2_RAISE_ERROR2(status,op)   \
    PJSUA2_RAISE_ERROR3(status, op, string())

/** Raise Error exception */
#   define PJSUA2_RAISE_ERROR3(status,op,txt)   \
    do { \
        Error err_ = Error(status, op, txt, string(), 0); \
        PJ_LOG(1,(THIS_FILE, "%s", err_.info().c_str())); \
        throw err_; \
    } while (0)

#endif

/** Raise Error exception if the expression fails */
#define PJSUA2_CHECK_RAISE_ERROR2(status, op)   \
    do { \
        if (status != PJ_SUCCESS) { \
        PJSUA2_RAISE_ERROR2(status, op); \
        } \
    } while (0)

/** Raise Error exception if the status fails */
#define PJSUA2_CHECK_RAISE_ERROR(status)    \
    PJSUA2_CHECK_RAISE_ERROR2(status, "")

/** Raise Error exception if the expression fails */
#define PJSUA2_CHECK_EXPR(expr)         \
    do { \
        pj_status_t the_status = expr;  \
        PJSUA2_CHECK_RAISE_ERROR2(the_status, #expr); \
    } while (0)

//////////////////////////////////////////////////////////////////////////////
/**
 * Version information.
 */
struct Version
{
    /** Major number */
    int     major;

    /** Minor number */
    int     minor;

    /** Additional revision number */
    int     rev;

    /** Version suffix (e.g. "-svn") */
    string  suffix;

    /** The full version info (e.g. "2.1.0-svn") */
    string  full;

    /**
     * PJLIB version number as three bytes with the following format:
     * 0xMMIIRR00, where MM: major number, II: minor number, RR: revision
     * number, 00: always zero for now.
     */
    unsigned    numeric;
};

}

//////////////////////////////////////////////////////////////////////////////

#define PJ2BOOL(var) ((var) != PJ_FALSE)

namespace pj
{
using std::string;

inline pj_str_t str2Pj(const string &input_str)
{
    pj_str_t output_str;
    output_str.ptr = (char*)input_str.c_str();
    output_str.slen = input_str.size();
    return output_str;
}

inline string pj2Str(const pj_str_t &input_str)
{
    if (input_str.ptr)
  return string(input_str.ptr, input_str.slen);
    return string();
}

/**
 * @}  PJSUA2
 */

} // namespace pj

#endif  /* __PJSETTINGS_TYPES_HPP__ */
