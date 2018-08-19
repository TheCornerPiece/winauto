import setuptools

with open("README.md", "r") as fh:
    long_description = fh.read()

setuptools.setup(
    name="winauto",
    version="0.0.1",
    author="TheCornerPiece",
    author_email="cornerpieceofficial@gmail.com",
    description="A simple Windows module to simulate user input (written in C)",
    long_description=long_description,
    long_description_content_type="text/markdown",
    url="https://github.com/thecornerpiece/winauto",
    packages=setuptools.find_packages(),
    classifiers=[
        "Programming Language :: Python :: 2",
        "License :: OSI Approved :: MIT License",
        "Operating System :: Windows",
    ],
)
