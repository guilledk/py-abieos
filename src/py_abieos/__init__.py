#!/usr/bin/env python3

import importlib

_abieos = importlib.import_module('_abieos')


Name = _abieos.Name


class ABIEOSError(Exception):
    ...


class ABIEOS:

    def __enter__(self):
        self.ctx = _abieos.abieos_create()
        return self

    def set_abi(self, abi: str, account: str = ''):
        _abieos.abieos_set_abi(
            self.ctx, int(Name(account)), abi)

    def _error_checked(self, fn, *args, **kwargs):
        result = fn(*args, **kwargs)

        if not result:
            raise ABIEOSError(
                _abieos.abieos_get_error(self.ctx)
            )

        return result

    def _fetch_result_buf(self, fn, *args, **kwargs) -> bytes:
        _ = self._error_checked(fn, *args, **kwargs)
        return self.ctx.result_bin

    def json_to_bin(self, type_name: str, json: str, account: str = '') -> bytes:
        return self._fetch_result_buf(
            _abieos.abieos_json_to_bin,
            self.ctx, int(Name(account)), type_name, json
        )

    def bin_to_json(self, type_name: str, json: str, account: str = '') -> str:
        return self._error_checked(
            _abieos.abieos_bin_to_json,
            self.ctx, int(Name(account)), type_name, json
        )

    def hex_to_json(self, type_name: str, hex_data: str, account: str = '') -> str:
        return self._error_checked(
            _abieos.abieos_hex_to_json,
            self.ctx, int(Name(account)), type_name, hex_data
        )

    def json_to_hex(self, type_name: str, json: str, account: str = '') -> str:
        return self._error_checked(
            _abieos.abieos_json_to_hex,
            self.ctx, int(Name(account)), type_name, json
        )

    def __exit__(self, exc_type, exc_value, traceback):
        _abieos.abieos_destroy(self.ctx)
