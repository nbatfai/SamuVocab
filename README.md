# SamuVocab, exp. 7, cognitive mental organs: MPU (Mental Processing Unit), acquiring higher-order knowledge

- The agent learns and recognizes a vocabulary of 20 words. It is shown in video at https://youtu.be/KGUrMSvD4RI
- The agent is learning and recognizing a vocabulary of 1200 words. It is shown in video at https://youtu.be/kjhJ7Q-7Ys0
- The agent has learned and recognized the first 375 of the 1200 words then it has run out of 16 Gb memory :(

I am looking for a definition of a mathematical machine for learning. 
The Mental Processing Unit (MPU) is a computational mental organ introduced 
in detail in the manuscript called "Samu in His Prenatal Development". This 
is a possible definition that is based on the Samu's COP-based Q-learning 
(see http://arxiv.org/abs/1511.02889) and on the well-known basic processes 
in infant development called habituation and sensitization. I have performed 
several experiments for creating the definition of MPUs. The SamuVocal in 
question is a strong simplification of the SamuBrain 
(see https://github.com/nbatfai/SamuBrain) because the MPU of SamuVocal uses 
only a vector of Samu's COP-based Q-learning instead of a matrix.

## Usage

```
git clone https://github.com/nbatfai/SamuVocab.git
cd SamuBrain/
~/Qt/5.5/gcc_64/bin/qmake SamuLife.pro
make
./SamuVocab 2>out
```

```
tail -f out|grep "HIGHER-ORDER NOTION MONITOR"
```

```
tail -f out|grep SENSITIZATION
```

```
tail -f out|grep "HABITUATION MONITOR" 
```

```
tail -f out|grep "WORD" 
```

## Experiments with this project

### Samu (Nahshon) has learned a vocabulary of 20 words

Samu has learned the words of the paper 'Baby’s First 10 Words' (see Table 4 in http://waldron.stanford.edu/~babylab/pdfs/Tardifetal2008.pdf)
This version is tagged by 'v20words'. The resulting can be found at https://youtu.be/KGUrMSvD4RI

### Samu (Nahshon) is learning words from Sitton's list of 1200 words

Rebecca Sitton's word list of 1200 high frequency words is used in GameOfLife.cpp.
This version is tagged by 'v1200words'. The resulting can be found at https://youtu.be/kjhJ7Q-7Ys0

### The agent has learned and recognized the first 375 of the 1200 words

![screenshot from 2016-02-22 19-29-05](https://cloud.githubusercontent.com/assets/3148120/13228894/8381a26c-d99e-11e5-91fe-5aeb1d47145e.png)

## Previous other experiments

Samu (Nahshon)
http://arxiv.org/abs/1511.02889,
https://github.com/nbatfai/nahshon

---

SamuLife
https://github.com/nbatfai/SamuLife,
https://youtu.be/b60m__3I-UM

SamuMovie
https://github.com/nbatfai/SamuMovie,
https://youtu.be/XOPORbI1hz4

SamuStroop
https://github.com/nbatfai/SamuStroop,
https://youtu.be/6elIla_bIrw,
https://youtu.be/VujHHeYuzIk

SamuBrain
https://github.com/nbatfai/SamuBrain

SamuCopy
https://github.com/nbatfai/SamuCopy

---

SamuTicker
https://github.com/nbatfai/SamuTicker

SamuVocab
https://github.com/nbatfai/SamuVocab
