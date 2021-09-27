# Chapter 0 - Introduction

## Purpose of this Course

This course is designed to take someone who has no experience in reverse engineering and bring them to a point where they will be able to reverse almost anything. From malware to video games, and from normal operating systems to reversing firmware. The course won't just show people how to do certain things but will try to teach the underlying logic and processes involved in each step.

## What is Reverse Engineering

Reverse Engineering is defined as ***the process of taking a piece of software or hardware and analyzing its functions and information flow so that its functionality and behavior can be understood.*** 

In reality, reverse engineering is often like finding a piece of hay in a needlestack. 

### What are the benefits of Reverse Engineering software?

Reverse engineering software allows us to find vulnerabilities and trade secrets. We can discover how malware behaves and write programs to stop the behavior before it can infect a system. We can patch programs and edit how the software behaves. Or, we can just reverse engineer something to satisfy our curiosity about how it works.

### Qualities of a Good Reverse Engineer:

Reverse engineering is a very time consuming and often infuriating process. You will end up spending hours combing through lines of code if you are lucky and even more hours combing through assembly if you are unlucky. And oftentimes you will need to write your own tools since if you are reverse engineering it for work or a bug bounty, chances are that you may be the first to try reversing it or someone else tried but didn't feel like sharing their tools.

Due to all of this, someone willing to brave the depths of reverse engineering needs to be patient, persistent, curious, and above all, needs to be able to think outside the box.

### What Do You Need to Know to Be a Good Reverse Engineer:

The ideal reverse engineer would know everything about everything, that however is sadly not possible. So lets focus on what we need to know and can actually achieve. 

A good reverse engineer should know at least one assembly language, luckily learning one assembly langauge is like learning C meaning that if we learn one we can do all the others with minimal effort.

As a reverse engineer you should also have a solid understanding of at least one programming language so that you can recognize things such as loops and conditionals within other languages. Having a solid understanding of pointers also helps a good bit.

You will also want a firm understanding of different operating systems such as syscalls and API's, data structures, and file structures. Oftentimes documented syscalls give us great starting points to how the program interacts with the user and system.

And lastly you will want good scripting skills. Once you have figured out what a program does you will often need to write scripts that can figure out things like inputs for specific outputs or sometimes you will need to write scripts to help you reverse engineer.

## Structure of the Course

The course will start with several chapters that must be done in order, these chapters include introduction to reverse engineering and its tools as well as basic file structures and assembly. After that, most chapters will be self contained and can be gone through in any order, there will however be a few subjects that span 2-3 chapters and should be done in order. These will all be marked as such.

Normally when it comes to a reverse engineering problem you want to start really broadly and then slowly narrow down to the important pieces, we will start by doing the opposite. The first few chapters of this course will start by focusing on the lowest level we can possibly go in software and getting an understanding of all the moving parts involved. Once we have gone over the basics and the tools we will go over specific lessons. 

The planned lessons range from specific tools and tricks for specific languages such as C#, C/C++, Java (and APK's), Python, and Objective C, to reversing cryptographic algorithms, WinAPI specific reversing, game hacking, packers and crypters, all the way to firmware hacking. 

At the end of the course one of the final projects will be writing a disassembler and subsequently a decompiler for Recurso, a programming language that I built.

# Chapter 1 - Assembly and Disassembly

# Chapter 2 - File Formats