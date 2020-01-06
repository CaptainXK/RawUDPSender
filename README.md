# RawUDPSender
## Target
Target Send a UDP segment via raw socket.

## Background

The skeleton of this code comes from this [link](https://blog.csdn.net/luchengtao11/article/details/73878760).

This trace sender is used to generate the adversarial packet trace described in current paper:<br>
**<\<Tuple Space Explosion: A Denial-of-Service Attack Against a Software Packet Classifier\>>**

When filling udp src_port and dst_port, the combination of these two fields are generated a outer-product of two lists.<br><br>
For example:
```
List_A : 
  a, b.
List_B : 
  e, f.
Then all combinations of <src_port, dst_port> are:
  <a, e>, <a, f>, <b, e>, <b, f>.
```

## The method of constructing adversarial trace:
Given a src_port, we start from the most significant bit and toggle one bit every time to generate a adversarial trace:<br><br>
For example:
```
Original port : 
  255 (1111 111)
All adversarial ports :
  127 (0111 1111)
  191 (1011 1111)
  223 (1101 1111)
  239 (1110 1111)
  247 (1111 0111)
  251 (1111 1011)
  253 (1111 1101)
  254 (1111 1110)
In the end, the list of ports will be constructed as followed:
  [255, 127, 191, 223, 239, 247, 251, 253, 254]
```
