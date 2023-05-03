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

The Physical (PHY) layer of the module was taken from the ns-3 LoRaWAN 
module (https://github.com/signetlabdei/lorawan or 
https://apps.nsnam.org/app/lorawan/).

LoRa Overview
*************

Module Design
*************

The module can be separated into two main parts: the PHY, and MAC layer. The
PHY layer is used to model the transmission and reception of LoRa signals
whereas the MAC layer models the routing of packets via a custom mesh
protocol.

Since, as mentioned above, the PHY layer was taken from the ns-3 LoRaWAN
module, this documentation focuses on the mesh aspect instead and would
recommend the LoRaWAN module's documentation for the explanation of the
PHY layer model.

MAC Layer Design (Mesh Protocol)
================================

The MAC layer model, inclusive of the custom mesh protocol, involves the
transmission/reception of three types of packets:

#. Routing Update,
#. Data Packets (the ones the nodes actually want to send), and
#. Feedback.

The Routing Update packets are sent by nodes in order to add to the 
routing tables of the other nodes which receive the packet. These
Routing Update packets contain information pertaining to either the
a known link between nodes or the existence of the node sending the
packet. The main metric used for assessing the quality of links is 
Estimated Transmission count (ETX) which is calculated based on the
number of previously received Routing Update packets.

The Data packets are kept in a queue to be sent, during the next
available timeslot, only when the pathfrom the source node to the
desitnation node is known based on the information in the nodes 
routing table, at that point in time. The timeslots are scheduled
to occur after a random delay (user specified range for RNG), following
which will be a routing update timeslot for sending a Routing Update
packet for a random entry in the node's routing table.

After having received a data packet, a node will then try to determine
if it has a lower ETX to the desintation compared to the last node which
forwarded the packet, if so it will transmit a feedback packet to that
last forwarding node to notify it that it will be forwarding the packet
itself. Upon receiving the feedback packet, the previous forwarding node
would then remove the packet from its queue. If, instead, no feedback
was received by a node sending a packet, it will continue to send up to
a fixed number of times.

Scope and Limitations
=====================

* Mesh Protocol Support
    
    Currently, the module only has support for the custom mesh protocol
    which was used as a proof of concept.
    
* Inter-protocol Interference & Inter-channel Interference

    These two limitations exist for the LoRaWAN module PHY layer so
    they would also apply for this module as it makes use of it.

Usage
*****

Helpers
=======

This module makes use of a helper to aid the user in collecting data on the
simulation through ascii tracing (``AsciiHelperForLoRa`` class). This was
done in place of PCAP tracing as the protocol would not be supported as it
was made alongside this module.

Examples
========

There are two examples included with the module:

* animal-tracking-scenario-example
    
    This example involves multiple mobile nodes (representing animals) moving
    constantly after being allocated near a central node which the mobile nodes
    attempt to send packets to.
    
* urban-scenario-example

    This example is based on test performed in Lee and Ke (2018). The scenario
    involved 20 nodes and a gateway in a university campus, with nodes attempting 
    to send packets to the gateway.

..  add info pertaining to tests
    
References
==========

Huang-Chen Lee, Kai-Hsiang Ke. 2018. "Monitoring of Large-Area IoT Sensors Using a LoRa 
Wireless Mesh Network System: Design and Evaluation". *IEEE Transactions on 
Instrumentation and Measurement* 67 (9).

LoRaWAN Module Links:

* Github Page - https://github.com/signetlabdei/lorawan
* App Store - https://apps.nsnam.org/app/lorawan/
