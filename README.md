# lora-mesh
LoRa mesh module for ns-3.29 which makes use of a custom mesh protocol

##  Building the Module
To build this module, you are required to have ns-3 (v3.29), see the [ns-3 wiki's guide](https://www.nsnam.org/wiki/Installation).

After which you can clone this repository to the `src` directory in your ns-3.29 directory:

```bash
git clone https://github.com/sanjay-charran/lora-mesh.git ns-3.29/src/lora-mesh
```

Lastly, configure your build with:

```bash
./waf configure --enable-examples --enable-tests
```

##  Tests
This module comes with five (5) ns-3 test suites which can be used to confirm it is working:

`lora-mesh-1`
`lora-mesh-2`
`lora-mesh-3`
`lora-mesh-4`
`lora-mesh-5`

These tests can be run using the method shown below:

```bash
./test.py -s lora-mesh-1
```
