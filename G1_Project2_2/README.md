README FOR P2
## MLFQ Execution Screenshot/Output
```text

+++ Multiprocessor Scheduling Simulator (MLFQ) +++
   Number of Processing Cores: 2
   Total tasks initialized: 15

--- Commencing MLFQ Scheduling ---
Queues: Q0(Q=2) -> Q1(Q=4) -> Q2(Q=8)
Tick	Core 0	Core 1	| External Wait States
----------------------------------------------------------
00	[Q0-T1]	[--]	| IO: None
01	[Q0-T1]	[Q0-T2]	| IO: None
02	[Q0-T3]	[Q0-T2]	| IO: None
03	[Q0-T3]	[Q1-T1]	| IO: [T2:2R] 
04	[Q0-T4]	[Q1-T1]	| IO: [T2:1R] 
05	[Q0-T4]	[Q1-T1]	| IO: None
06	[Q0-T2]	[Q1-T1]	| IO: None
07	[Q0-T2]	[Q0-T5]	| IO: None
08	[Q0-T6]	[Q0-T5]	| IO: [T2:2R] 
09	[Q0-T6]	[Q1-T3]	| IO: [T2:1R] 
10	[Q0-T2]	[Q1-T3]	| IO: None
11	[Q0-T2]	[Q0-T7]	| IO: [T3:4R] 
12	[Q0-T8]	[Q0-T7]	| IO: [T3:3R] 
13	[Q0-T8]	[Q1-T4]	| IO: [T3:2R] 
14	[Q1-T5]	[Q1-T4]	| IO: [T3:1R] 
15	[Q1-T5]	[Q0-T3]	| IO: None
16	[Q1-T5]	[Q0-T3]	| IO: None
17	[Q1-T5]	[Q0-T9]	| IO: None
18	[Q0-T10]	[Q0-T9]	| IO: None
19	[Q0-T10]	[Q1-T3]	| IO: None
20	[Q0-T11]	[Q1-T3]	| IO: None
21	[Q1-T6]	[Q1-T7]	| IO: [T3:4R] [T11:1R] 
22	[Q1-T6]	[Q0-T11]	| IO: [T3:3R] [T7:1R] 
23	[Q1-T6]	[Q0-T7]	| IO: [T3:2R] [T11:1R] 
24	[Q0-T11]	[Q0-T7]	| IO: [T3:1R] 
25	[Q0-T3]	[Q0-T12]	| IO: None
26	[Q0-T3]	[Q0-T12]	| IO: None
27	[Q0-T13]	[Q1-T7]	| IO: None
28	[Q0-T13]	[Q0-T14]	| IO: [T7:1R] 
29	[Q0-T7]	[Q0-T14]	| IO: None
30	[Q0-T7]	[Q0-T15]	| IO: None
31	[Q1-T7]	[Q0-T15]	| IO: None
32	[Q1-T8]	[Q1-T9]	| IO: [T7:1R] 
33	[Q1-T8]	[Q1-T9]	| IO: None
34	[Q1-T8]	[Q1-T9]	| IO: None
35	[Q1-T8]	[Q0-T7]	| IO: [T9:5R] 
36	[Q1-T10]	[Q0-T7]	| IO: [T9:4R] 
37	[Q1-T10]	[Q1-T7]	| IO: [T9:3R] 
38	[Q1-T10]	[Q1-T12]	| IO: [T9:2R] 
39	[Q1-T10]	[Q1-T12]	| IO: [T9:1R] 
40	[Q0-T9]	[Q1-T12]	| IO: None
41	[Q0-T9]	[Q1-T12]	| IO: None
42	[Q1-T9]	[Q1-T13]	| IO: None
43	[Q1-T9]	[Q1-T14]	| IO: [T13:2R] 
44	[Q1-T9]	[Q1-T14]	| IO: [T13:1R] 
45	[Q0-T13]	[Q1-T14]	| IO: [T9:5R] 
46	[Q0-T13]	[Q1-T14]	| IO: [T9:4R] 
47	[Q1-T13]	[Q1-T15]	| IO: [T9:3R] [T14:3R] 
48	[Q2-T1]	[Q1-T15]	| IO: [T9:2R] [T13:2R] [T14:2R] 
49	[Q2-T1]	[Q1-T15]	| IO: [T9:1R] [T13:1R] [T14:1R] 
50	[Q2-T1]	[Q0-T9]	| IO: None
51	[Q2-T1]	[Q0-T9]	| IO: None
52	[Q2-T1]	[Q0-T13]	| IO: None
53	[Q2-T1]	[Q0-T14]	| IO: None
54	[Q2-T1]	[Q0-T14]	| IO: None
55	[Q2-T1]	[Q1-T9]	| IO: None
56	[Q1-T14]	[Q1-T9]	| IO: None
57	[Q1-T14]	[Q1-T9]	| IO: None
58	[Q1-T14]	[Q2-T1]	| IO: [T9:5R] 
59	[Q1-T14]	[Q2-T5]	| IO: [T9:4R] 
60	[Q2-T8]	[Q2-T5]	| IO: [T9:3R] [T14:3R] 
61	[Q2-T8]	[Q2-T10]	| IO: [T5:3R] [T9:2R] [T14:2R] 
62	[Q2-T12]	[Q2-T10]	| IO: [T5:2R] [T9:1R] [T14:1R] 
63	[Q2-T12]	[Q2-T10]	| IO: [T5:1R] 
64	[Q2-T12]	[Q0-T5]	| IO: None
65	[Q2-T12]	[Q0-T5]	| IO: None
66	[Q0-T9]	[Q0-T14]	| IO: [T12:4R] 
67	[Q0-T9]	[Q0-T14]	| IO: [T12:3R] 
68	[Q1-T5]	[Q1-T9]	| IO: [T12:2R] 
69	[Q1-T5]	[--]	| IO: [T12:1R] 
70	[Q1-T5]	[Q0-T12]	| IO: None
71	[Q1-T5]	[Q0-T12]	| IO: None
72	[Q1-T12]	[Q2-T5]	| IO: None
73	[Q1-T12]	[Q2-T5]	| IO: None
74	[Q1-T12]	[--]	| IO: [T5:3R] 
75	[Q1-T12]	[--]	| IO: [T5:2R] 
76	[Q2-T12]	[--]	| IO: [T5:1R] 
77	[Q2-T12]	[Q0-T5]	| IO: None
78	[Q2-T12]	[Q0-T5]	| IO: None
79	[Q2-T12]	[Q1-T5]	| IO: None
80	[--]	[Q1-T5]	| IO: [T12:4R] 
81	[--]	[--]	| IO: [T12:3R] 
82	[--]	[--]	| IO: [T12:2R] 
83	[--]	[--]	| IO: [T12:1R] 
84	[Q0-T12]	[--]	| IO: None
85	[Q0-T12]	[--]	| IO: None
86	[Q1-T12]	[--]	| IO: None
87	[Q1-T12]	[--]	| IO: None
88	[Q1-T12]	[--]	| IO: None

--- Performance Stats ---
TID	Arrival	Burst	Start	Finish	Turnaround	Waiting
---	-------	-----	-----	------	----------	-------
T1	0	15	0	59	59		44
T2	1	6	1	12	11		5
T3	2	10	2	27	25		15
T4	4	4	4	15	11		7
T5	5	20	7	81	76		56
T6	8	5	8	24	16		11
T7	10	12	11	38	28		16
T8	12	8	12	62	50		42
T9	15	18	17	69	54		36
T10	18	9	18	64	46		37
T11	20	3	20	25	5		2
T12	22	25	25	89	67		42
T13	25	7	27	53	28		21
T14	28	14	28	68	40		26
T15	30	5	30	50	20		15

Mean Turnaround Time : 35.73
Mean Waiting Time    : 25.00
```
