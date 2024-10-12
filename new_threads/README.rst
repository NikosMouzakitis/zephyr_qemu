.. zephyr:code-sample:: multi-thread-blinky
   :name: Basic thread manipulation
   :relevant-api: gpio_interface thread_apis

   Spawn multiple threads that blink LEDs and print information to the console.

Overview
********

This example demonstrates spawning multiple threads using
:c:func:`K_THREAD_DEFINE`. It spawns three threads. Each thread is then defined
at compile time using K_THREAD_DEFINE.

- ``blink0()`` controls ``led0`` 
- ``blink1()`` controls ``led1`` we assign them different clock cycles.

When either of these threads toggles its virtual LED, it also pushes information into a
:ref:`FIFO <fifos_v2>` identifying the thread/LED and how many times it has
been toggled.

The third thread uses :c:func:`printk` to print the information added to the
FIFO to the device console.

Requirements
************
we run it for riscv32 on qemu.
Building
********
refer to zephyr_installation.txt
