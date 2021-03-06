///////////////////////////////////////////////////////////////////////////////
//
// SigningRequest.h
//
// Copyright (c) 2014 Eric Lombrozo
//
// All Rights Reserved.
//

#pragma once

#include <CoinQ/CoinQ_typedefs.h>

#include <set>

namespace CoinDB
{

class SigningRequest
{
public:
    typedef std::pair<std::string, bytes_t> keychain_info_t;
    SigningRequest(unsigned int sigs_needed, const std::set<keychain_info_t>& keychain_info, const bytes_t& rawtx = bytes_t()) :
        sigs_needed_(sigs_needed), keychain_info_(keychain_info), rawtx_(rawtx) { }

    unsigned int sigs_needed() const { return sigs_needed_; }
    const std::set<keychain_info_t>& keychain_info() const { return keychain_info_; }
    const bytes_t& rawtx() const { return rawtx_; }

private:
    unsigned int sigs_needed_;
    std::set<keychain_info_t> keychain_info_;
    bytes_t rawtx_;
};

}
