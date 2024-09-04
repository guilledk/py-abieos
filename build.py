from setuptools import setup
from setuptools.command.build_py import build_py as build_py_orig
import shutil
import os

class CustomBuildCommand(build_py_orig):
    def run(self):
        # First, run the original build
        super().run()

        # Now copy the .so file to the destination inside the wheel
        source = 'src/py_abieos/_abieos.so'
        destination = os.path.join(self.build_lib, 'py_abieos', '_abieos.so')

        self.mkpath(os.path.dirname(destination))
        shutil.copyfile(source, destination)
        print(f'Copied {source} to {destination}')

setup(
    cmdclass={
        'build_py': CustomBuildCommand,
    }
)
