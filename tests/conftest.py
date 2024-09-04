

import pytest

from py_abieos import _abieos, ABIEOS


@pytest.fixture
def ship_abi():
    with open('tests/ship_abi.json', 'r') as abi_file:
        yield abi_file.read()


@pytest.fixture
def abieos_ctx():
    ctx = _abieos.abieos_create()
    yield ctx
    _abieos.abieos_destroy(ctx)


@pytest.fixture
def abieos(ship_abi):
    with ABIEOS() as abieos:
        abieos.set_abi(ship_abi)
        yield abieos
