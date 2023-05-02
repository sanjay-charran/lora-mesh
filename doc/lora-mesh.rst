LoRa Mesh Module
----------------

.. include:: replace.txt
.. highlight:: cpp

.. heading hierarchy:
   ------------- Chapter
   ************* Section (#.#)
   ============= Subsection (#.#.#)
   ############# Paragraph (no number)
   
This module allows for the simulation of mesh topologies making use of LoRa
modulation. It makes use of a custom mesh protocol which uses ETX (Estimated 
Transmission Count) to route packets.

The Physical (PHY) layer of the module was ported over from the ns-3 LoRaWAN 
module (https://github.com/signetlabdei/lorawan or 
https://apps.nsnam.org/app/lorawan/).
Validation
**********

Describe how the model has been tested/validated.  What tests run in the
test suite?  How much API and code is covered by the tests?  Again, 
references to outside published work may help here.
Module Design
*************
.. general class structure
The source code for the new module lives in the directory ``src/lora-mesh``.

Add here a basic description of what is being modeled.

PHY Layer Design
================


MAC Layer Design (Mesh Protocol)
================================


Scope and Limitations
=====================

What can the model do?  What can it not do?  Please use this section to
describe the scope and limitations of the model.

References
==========

Add academic citations here, such as if you published a paper on this
model, or if readers should read a particular specification or other work.

Usage
*****

This section is principally concerned with the usage of your model, using
the public API.  Focus first on most common usage patterns, then go
into more advanced topics.

Building LoRa Mesh Module
=========================

Include this subsection only if there are special build instructions or
platform limitations.

Helpers
=======

What helper API will users typically use?  Describe it here.

Attributes
==========

What classes hold attributes, and what are the key ones worth mentioning?

Output
======

What kind of data does the model generate?  What are the key trace
sources?   What kind of logging output can be enabled?

Advanced Usage
==============

Go into further details (such as using the API outside of the helpers)
in additional sections, as needed.

Examples
========

What examples using this new code are available?  Describe them here.

Tests
=====

Troubleshooting
===============

Add any tips for avoiding pitfalls, etc.
