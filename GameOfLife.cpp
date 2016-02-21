/**
 * @brief Samu has learnt the rules of Conway's Game of Life
 *
 * @file GameOfLife.h
 * @author  Norbert Bátfai <nbatfai@gmail.com>
 * @version 0.0.1
 *
 * @section LICENSE
 *
 * Copyright (C) 2015, 2016 Norbert Bátfai, batfai.norbert@inf.unideb.hu
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @section DESCRIPTION
 *
 * SamuBrain, exp. 4, cognitive mental organs: MPU (Mental Processing Unit), Q-- lerning, acquiring higher-order knowledge
 *
 * This is an example of the paper entitled "Samu in his prenatal development".
 *
 * Previous experiments
 *
 * Samu (Nahshon)
 * http://arxiv.org/abs/1511.02889
 * https://github.com/nbatfai/nahshon
 *
 * SamuLife
 * https://github.com/nbatfai/SamuLife
 * https://youtu.be/b60m__3I-UM
 *
 * SamuMovie
 * https://github.com/nbatfai/SamuMovie
 * https://youtu.be/XOPORbI1hz4
 *
 * SamuStroop
 * https://github.com/nbatfai/SamuStroop
 * https://youtu.be/6elIla_bIrw
 * https://youtu.be/VujHHeYuzIk
 */


#include "GameOfLife.h"

GameOfLife::GameOfLife ( int w, int h ) : m_w ( w ), m_h ( h )
{

  lattices = new int**[2];

  lattices[0] = new int*[m_h];
  for ( int i {0}; i<m_h; ++i )
    {
      lattices[0][i] = new int [m_w];
    }

  lattices[1] = new int*[m_h];

  for ( int i {0}; i<m_h; ++i )
    {
      lattices[1][i] = new int [m_w];
    }

  predictions = new int*[m_h];

  for ( int i {0}; i<m_h; ++i )
    {
      predictions[i] = new int [m_w];
    }

  latticeIndex = 0;

  int ** lattice = lattices[latticeIndex];

  for ( int i {0}; i<m_h; ++i )
    for ( int j {0}; j<m_w; ++j )
      {
        lattice[i][j] = 0;
      }

  samuBrain = new SamuBrain ( m_w, m_h );

  carx = 0;
  manx = m_w/2;
  housex = 2*m_w/5;

}

GameOfLife::~GameOfLife()
{
  for ( int i {0}; i<m_h; ++i )
    {
      delete[] lattices[0][i];
      delete[] lattices[1][i];
      delete[] predictions[i];
    }

  delete[] predictions;
  delete[] lattices[0];
  delete[] lattices[1];
  delete[] lattices;

  delete samuBrain;
}


int ** GameOfLife::lattice()
{
  return lattices[latticeIndex];
}

void GameOfLife::run()
{

  int **fp, **fr;
  while ( true )
    {
      QThread::msleep ( m_delay );

      if ( !paused )
        {

          ++m_time;

          qDebug() << "<<<" << m_time << "<<<";

          development();

          if ( samuBrain )
            {
              samuBrain->learning ( lattices[latticeIndex], predictions, &fp, &fr );
              qDebug() << m_time
                       << "   #MPUs:" << samuBrain->nofMPUs()
                       << "Observation (MPU):" << samuBrain->get_foobar().c_str();
            }

          latticeIndex = ( latticeIndex+1 ) %2;
          emit cellsChanged ( lattices[latticeIndex], predictions, fp, fr );

          qDebug() << ">>>" << m_time << ">>>";

        }
    }

}

void GameOfLife::pause()
{
  paused = !paused;
}

int GameOfLife::numberOfNeighbors ( int **lattice, int r, int c, int state )
{
  int number {0};

  for ( int i {-1}; i<2; ++i )
    for ( int j {-1}; j<2; ++j )

      if ( ! ( ( i==0 ) && ( j==0 ) ) )
        {
          int o = c + j;
          if ( o < 0 )
            {
              o = m_w-1;
            }
          else if ( o >= m_w )
            {
              o = 0;
            }

          int s = r + i;
          if ( s < 0 )
            {
              s = m_h-1;
            }
          else if ( s >= m_h )
            {
              s = 0;
            }

          if ( lattice[s][o] == state )
            {
              ++number;
            }
        }

  return number;
}


void GameOfLife::clear_lattice ( int **nextLattice )
{
  for ( int i {0}; i<m_h; ++i )
    for ( int j {0}; j<m_w; ++j )
      {
        nextLattice[i][j] = 0;
      }
}

void GameOfLife::fill_lattice ( int **nextLattice, int color )
{
  for ( int i {0}; i<m_h; ++i )
    for ( int j {0}; j<m_w; ++j )
      {
        nextLattice[i][j] = color;
      }
}

void GameOfLife::control_Conway ( int **prevLattice, int **nextLattice )
{
  for ( int i {0}; i<m_h; ++i )

    for ( int j {0}; j<m_w; ++j )
      {

        int liveNeighbors = numberOfNeighbors ( prevLattice, i, j, true );

        if ( prevLattice[i][j] == true )
          {
            if ( liveNeighbors==2 || liveNeighbors==3 )
              {
                nextLattice[i][j] = true;
              }
            else
              {
                nextLattice[i][j] = false;
              }
          }
        else
          {
            if ( liveNeighbors==3 )
              {
                nextLattice[i][j] = true;
              }
            else
              {
                nextLattice[i][j] = false;
              }
          }
      }
}

void GameOfLife::control_Movie ( int **nextLattice )
{
  if ( m_time %3 ==0 )
    {

      if ( carx < m_w-5 )
        {
          carx += 2;
        }
      else
        {
          carx = 0;
        }
    }

  if ( m_time %6 ==0 )
    {
      if ( manx < m_w-3 )
        {
          ++manx;
        }
      else
        {
          manx = 0;
        }
    }

  house ( nextLattice, housex, 3*m_h/5 -6 );
  car ( nextLattice, carx, 3*m_h/5 +1 );
  man ( nextLattice, manx, 3*m_h/5-1 );

}

void GameOfLife::control_Stroop ( int **nextLattice )
{
  if ( ++age <20 )
    {
      red ( nextLattice, 2, 5, 1 );
    }
  else if ( age <40 )
    {
      green ( nextLattice, 2, 5, 1 );
    }
  else if ( age <60 )
    {
      blue ( nextLattice, 2, 5, 1 );
    }
  else if ( age <80 )
    {
      red ( nextLattice, 2, 5, 2 );
    }
  else if ( age <100 )
    {
      green ( nextLattice, 2, 5, 2 );
    }
  else if ( age <120 )
    {
      blue ( nextLattice, 2, 5, 2 );
    }
  else if ( age <140 )
    {
      red ( nextLattice, 2, 5, 3 );
    }
  else if ( age <160 )
    {
      green ( nextLattice, 2, 5, 3 );
    }
  else if ( age <180 )
    {
      blue ( nextLattice, 2, 5, 3 );
    }
  else
    {
      age =0;
      red ( nextLattice, 2, 5, 2 );
    }

}

void GameOfLife::ticker ( int **lattice, std::string & hello )
{
  static int xx = 34;//hello.length();

  int l = hello.length();

  for ( int i {0}; i<l; ++i )
    {
      if ( xx+i >= 0 && xx+i < m_w )
        {

          char c = hello[i];
          lattice[0][xx+i] = c;

        }
    }

  --xx;
  if ( xx< ( -1*l ) )
    xx= 34;//hello.length();

}


void GameOfLife::development()
{

  int **prevLattice = lattices[latticeIndex];
  int **nextLattice = lattices[ ( latticeIndex+1 ) %2];

  std::string hello1 = "I am Samu. I am a disembodied developmental robotic agent.";
  std::string hello2 = "I am Norbi Batfai. I am an embodied human being from Earth.";

  /*
  std::vector<std::string> hello =
  {
    // http://waldron.stanford.edu/~babylab/pdfs/Tardifetal2008.pdf
    "Daddy",
    "Mommy",
    "BaaBaa",
    "Bye",
    "Hi",
    "UhOh",
    "Grr",
    "Bottle",
    "YumYum",
    "Dog",
    "No",
    "WoofWoof",
    "Vroom",
    "Kitty",
    "Ball",
    "Baby",
    "Duck",
    "Cat",
    "Ouch",
    "Banana"
  };
*/
  std::vector<std::string> hello =  
  {
    // Rebecca Sitton's word list of 1200 high frequency words 
    "the",
"of",
"and",
"a",
"to",
"in",
"is",
"you",
"that",
"it",
"he",
"for",
"was",
"on",
"are",
"as",
"with",
"his",
"they",
"at",
"be",
"this",
"from",
"I",
"have",
"or",
"by",
"one",
"had",
"not",
"but",
"what",
"all",
"were",
"when",
"we",
"there",
"can",
"an",
"your",
"which",
"their",
"said",
"if",
"do",
"will",
"each",
"about",
"how",
"up",
"out",
"them",
"then",
"she",
"many",
"some",
"so",
"these",
"would",
"other",
"into",
"has",
"more",
"her",
"two",
"like",
"him",
"see",
"time",
"could",
"no",
"make",
"than",
"first",
"been",
"its",
"who",
"now",
"people",
"my",
"made",
"over",
"did",
"down",
"only",
"way",
"find",
"use",
"may",
"water",
"long",
"little",
"very",
"after",
"words",
"called",
"just",
"where",
"most",
"know",
"get",
"through",
"back",
"much",
"go",
"good",
"new",
"write",
"our",
"me",
"man",
"too",
"any",
"day",
"same",
"right",
"look",
"think",
"also",
"around",
"another",
"came",
"come",
"work",
"three",
"must",
"because",
"does",
"part",
"even",
"place",
"well",
"such",
"here",
"take",
"why",
"help",
"put",
"different",
"away",
"again",
"off",
"went",
"old",
"number",
"great",
"tell",
"men",
"say",
"small",
"every",
"found",
"still",
"between",
"mane",
"should",
"home",
"big",
"give",
"air",
"line",
"set",
"own",
"under",
"read",
"last",
"never",
"us",
"left",
"end",
"along",
"while",
"might",
"next",
"sound",
"below",
"saw",
"something",
"thought",
"both",
"few",
"those",
"always",
"show",
"large",
"often",
"together",
"asked",
"house",
"don't",
"world",
"going",
"want",
"school",
"important",
"until",
"form",
"food",
"keep",
"children",
"feet",
"land",
"side",
"without",
"boy",
"once",
"animal",
"life",
"enough",
"took",
"four",
"head",
"above",
"kind",
"began",
"almost",
"live",
"page",
"got",
"earth",
"need",
"far",
"hand",
"high",
"year",
"mother",
"light",
"country",
"father",
"let",
"night",
"picture",
"being",
"study",
"second",
"soon",
"story",
"since",
"white",
"ever",
"paper",
"hard",
"near",
"sentence",
"better",
"best",
"across",
"during",
"today",
"however",
"sure",
"knew",
"it's",
"try",
"told",
"young",
"sun",
"thing",
"whole",
"hear",
"example",
"heard",
"several",
"change",
"answer",
"room",
"sea",
"against",
"top",
"turned",
"learn",
"point",
"city",
"play",
"toward",
"five",
"himself",
"usually",
"money",
"seen",
"didn't",
"car",
"morning",
"I'm",
"body",
"upon",
"family",
"later",
"turn",
"move",
"face",
"door",
"cut",
"done",
"group",
"true",
"half",
"red",
"fish",
"plants",
"living",
"black",
"eat",
"short",
"United",
"run",
"book",
"gave",
"order",
"open",
"ground",
"cold",
"really",
"table",
"remember",
"tree",
"course",
"front",
"American",
"space",
"inside",
"ago",
"sad",
"early",
"I'll",
"learned",
"brought",
"close",
"nothing",
"though",
"idea",
"before",
"lived",
"became",
"add",
"become",
"grow",
"draw",
"yet",
"less",
"wind",
"behind",
"cannot",
"letter",
"among",
"able",
"dog",
"shown",
"mean",
"English",
"rest",
"perhaps",
"certain",
"six",
"feel",
"fire",
"ready",
"green",
"yes",
"built",
"special",
"ran",
"full",
"town",
"complete",
"oh",
"person",
"hot",
"anything",
"hold",
"state",
"list",
"stood",
"hundred",
"ten",
"fast",
"felt",
"kept",
"notice",
"can't",
"strong",
"voice",
"probably",
"area",
"horse",
"matter",
"stand",
"box",
"start",
"that's",
"class",
"piece",
"surface",
"river",
"common",
"stop",
"am",
"talk",
"whether",
"fine",
"round",
"dark",
"past",
"ball",
"girl",
"road",
"blue",
"instead",
"either",
"held",
"already",
"warm",
"gone",
"finally",
"summer",
"understand",
"moon",
"animals",
"mind",
"outside",
"power",
"problem",
"longer",
"winter",
"deep",
"heavy",
"carefully",
"follow",
"beautiful",
"everyone",
"leave",
"everything",
"game",
"system",
"bring",
"watch",
"shell",
"dry",
"within",
"floor",
"ice",
"ship",
"themselves",
"begin",
"fact",
"third",
"quite",
"carry",
"distance",
"although",
"sat",
"possible",
"heart",
"real",
"simple",
"snow",
"rain",
"suddenly",
"easy",
"leaves",
"lay",
"size",
"wild",
"weather",
"miss",
"pattern",
"sky",
"walked",
"main",
"someone",
"center",
"field",
"stay",
"itself",
"boat",
"question",
"wide",
"least",
"tiny",
"hour",
"happened",
"foot",
"care",
"low",
"else",
"gold",
"build",
"glass",
"rock",
"tall",
"alone",
"bottom",
"check",
"reading",
"fall",
"poor",
"map",
"friend",
"language",
"job",
"music",
"buy",
"window",
"mark",
"heat",
"grew",
"listen",
"ask",
"single",
"clear",
"energy",
"week",
"explain",
"lost",
"spring",
"travel",
"wrote",
"farm",
"circle",
"whose",
"correct",
"bed",
"measure",
"straight",
"base",
"mountain",
"caught",
"hair",
"bird",
"wood",
"color",
"war",
"fly",
"yourself",
"seem",
"thus",
"square",
"moment",
"teacher",
"happy",
"bright",
"sent",
"present",
"plan",
"rather",
"length",
"speed",
"machine",
"information",
"except",
"figure",
"you're",
"free",
"fell",
"suppose",
"natural",
"ocean",
"government",
"baby",
"grass",
"plane",
"street",
"couldn't",
"reason",
"difference",
"maybe",
"history",
"mouth",
"middle",
"step",
"child",
"strange",
"wish",
"soil",
"human",
"trip",
"woman",
"eye",
"milk",
"choose",
"north",
"seven",
"famous",
"late",
"pay",
"sleep",
"iron",
"trouble",
"store",
"beside",
"oil",
"modern",
"fun",
"catch",
"business",
"reach",
"lot",
"won't",
"case",
"speak",
"shape",
"eight",
"edge",
"soft",
"village",
"object",
"age",
"minute",
"wall",
"meet",
"record",
"copy",
"forest",
"especially",
"necessary",
"he's",
"unit",
"flat",
"direction",
"south",
"subject",
"skin",
"wasn't",
"I've",
"yellow",
"party",
"force",
"test",
"bad",
"temperature",
"pair",
"ahead",
"wrong",
"practice",
"sand",
"tail",
"wait",
"difficult",
"general",
"cover",
"material",
"isn't",
"thousand",
"sign",
"guess",
"forward",
"huge",
"ride",
"region",
"nor",
"period",
"blood",
"rich",
"team",
"corner",
"cat",
"amount",
"garden",
"led",
"note",
"various",
"race",
"bit",
"result",
"brother",
"addition",
"doesn't",
"dead",
"weight",
"thin",
"stone",
"hit",
"wife",
"island",
"we'll",
"opposite",
"born",
"sense",
"cattle",
"million",
"anyone",
"rule",
"science",
"afraid",
"women",
"produce",
"pull",
"son",
"meant",
"broken",
"interest",
"chance",
"thick",
"sight",
"pretty",
"train",
"fresh",
"drive",
"lead",
"break",
"sit",
"bought",
"radio",
"method",
"king",
"similar",
"return",
"corn",
"decide",
"position",
"bear",
"hope",
"song",
"engine",
"board",
"control",
"spread",
"evening",
"brown",
"clean",
"wouldn't",
"section",
"spent",
"ring",
"teeth",
"quiet",
"ancient",
"stick",
"afternoon",
"silver",
"nose",
"century",
"therefore",
"level",
"you'll",
"death",
"hole",
"coast",
"cross",
"sharp",
"fight",
"capital",
"fill",
"deal",
"busy",
"beyond",
"send",
"love",
"cool",
"cause",
"please",
"meat",
"lady",
"west",
"glad",
"action",
"pass",
"type",
"attention",
"gas",
"kitchen",
"pick",
"scale",
"basic",
"happen",
"safe",
"grown",
"cost",
"wear",
"act",
"hat",
"arm",
"believe",
"major",
"gray",
"wonder",
"include",
"describe",
"electric",
"sold",
"visit",
"sheep",
"I'd",
"office",
"row",
"contain",
"fit",
"equal",
"value",
"yard",
"beat",
"inch",
"sugar",
"key",
"product",
"desert",
"bank",
"farther",
"won",
"total",
"sell",
"wire",
"rose",
"cotton",
"spoke",
"rope",
"fear",
"shore",
"throughout",
"compare",
"movement",
"exercise",
"bread",
"process",
"nature",
"apart",
"path",
"careful",
"narrow",
"mental",
"nine",
"useful",
"public",
"according",
"steel",
"salt",
"speech",
"forth",
"nation",
"knowledge",
"appear",
"ate",
"dinner",
"hurt",
"spend",
"experiment",
"touch",
"drop",
"chair",
"east",
"separate",
"truck",
"sing",
"column",
"twice",
"particular",
"shop",
"unless",
"spot",
"neither",
"met",
"wheel",
"none",
"hill",
"television",
"bill",
"solve",
"pressure",
"report",
"farmer",
"count",
"trade",
"chief",
"month",
"clothes",
"doctor",
"indeed",
"dance",
"church",
"original",
"enjoy",
"string",
"sister",
"familiar",
"onto",
"imagine",
"blow",
"quick",
"law",
"lie",
"final",
"rise",
"loud",
"fair",
"herself",
"slow",
"noise",
"statement",
"hungry",
"join",
"tube",
"rode",
"empty",
"twenty",
"broke",
"nice",
"effect",
"paid",
"motion",
"myself",
"divide",
"supply",
"laid",
"dear",
"surprise",
"gun",
"entire",
"fruit",
"crowd",
"band",
"wet",
"solid",
"northern",
"flower",
"star",
"feed",
"wooden",
"sort",
"develop",
"shoulder",
"variety",
"season",
"share",
"jump",
"regular",
"represent",
"market",
"we're",
"flew",
"finger",
"expect",
"army",
"cabin",
"camp",
"danger",
"purpose",
"breakfast",
"proper",
"coat",
"push",
"express",
"shot",
"angry",
"southern",
"dress",
"bag",
"proud",
"neck",
"breath",
"strength",
"member",
"twelve",
"mine",
"company",
"current",
"pound",
"valley",
"double",
"till",
"match",
"average",
"die",
"liquid",
"alive",
"stream",
"provide",
"drink",
"experience",
"future",
"tomorrow",
"drove",
"population",
"finish",
"station",
"shook",
"stage",
"oxygen",
"poem",
"solution",
"burn",
"cent",
"electricity",
"everybody",
"rate",
"dust",
"worth",
"community",
"captain",
"bus",
"protect",
"cook",
"raise",
"further",
"steam",
"guide",
"discover",
"plain",
"usual",
"seat",
"accept",
"police",
"consider",
"dozen",
"baseball",
"rubber",
"symbol",
"support",
"exactly",
"industry",
"they're",
"beneath",
"laugh",
"groceries",
"popular",
"thank",
"quarter",
"climbed",
"continue",
"potatoes",
"receive",
"design",
"president",
"charge",
"mistake",
"hospital",
"remain",
"service",
"increase",
"students",
"insects",
"address",
"sincerely",
"dollars",
"belong",
"bottle",
"flight",
"forget",
"bicycle",
"secret",
"soldier",
"silent",
"structure",
"height",
"observe",
"indicate",
"railroad",
"knife",
"married",
"suggested",
"entered",
"magazine",
"agree",
"fifty",
"escape",
"threw",
"planet",
"dangerous",
"event",
"leader",
"peace",
"spelling",
"chapter",
"swimming",
"opportunity",
"immediately",
"favorite",
"settled",
"telephone",
"repeat",
"prepare",
"instance",
"avenue",
"newspaper",
"actually",
"employee",
"review",
"convince",
"allowed",
"nobody",
"details",
"muscles",
"model",
"climate",
"coffee",
"whenever",
"serious",
"angle",
"feather",
"determined",
"dictionary",
"ordinary",
"extra",
"rough",
"library",
"condition",
"arrived",
"located",
"program",
"pencil",
"tongue",
"title",
"enemy",
"garage",
"lose",
"vegetable",
"parents",
"style",
"education",
"required",
"political",
"daughter",
"individual",
"progress",
"altogether",
"activities",
"article",
"equipment",
"discuss",
"healthy",
"perfect",
"recognize",
"frequently",
"character",
"personal",
"disappear",
"success",
"traffic",
"yesterday",
"situation",
"realize",
"message",
"recently",
"account",
"physical",
"neighbor",
"excited",
"whisper",
"available",
"college",
"furniture",
"leather",
"husband",
"principal",
"medicine",
"excellent",
"operation",
"council",
"author",
"organize",
"concern",
"barbecue",
"accident",
"disease",
"construction",
"motor",
"affect",
"conversation",
"evidence",
"citizen",
"environment",
"influence",
"cancel",
"audience",
"apartment",
"worse",
"transportation",
"frozen",
"waste",
"couple",
"function",
"connect",
"project",
"pronounce",
"offered",
"apply",
"improve",
"stomach",
"collect",
"prevent",
"courage",
"occur",
"foreign",
"quality",
"terrible",
"instrument",
"balance",
"ability",
"arrange",
"rhythm",
"avoid",
"daily",
"identity",
"standard",
"combine",
"attached",
"frighten",
"social",
"factory",
"license",
"recommend"};


  clear_lattice ( nextLattice );

  int ind = ( m_time/6000 ) % hello.size();
  qDebug() << m_time
           << "   WORD:" << ind << hello[ind].c_str()
           << "Observation (MPU):" << samuBrain->get_foobar().c_str();

  ticker ( nextLattice, hello[ind] );

  /*
  if ( m_time < 12000 )
    {
    ticker ( nextLattice, hello1 );
  }
  else if ( m_time < 34000 )
    {
  ticker ( nextLattice, hello2);
    }
  else
    {
      m_time = -1;
    }
  */

  /*

  if ( m_time == 1 )
    {
      //clear_lattice ( nextLattice );

      glider ( nextLattice, 2*m_w/5, 2*m_h/5 );
      glider ( nextLattice, 3*m_w/5, 3*m_h/5 );
      glider ( nextLattice, 4*m_w/5, 4*m_h/5 );
      glider ( nextLattice, 4*m_w/5, 2*m_h/5 );
      glider ( nextLattice, 2*m_w/5, 4*m_h/5 );

    }
  else if ( m_time < 5000 )
    {
      control_Conway ( prevLattice, nextLattice );
    }
  else if ( m_time < 13000 )
    {
      control_Stroop ( nextLattice );
    }
  else if ( m_time < 22000 )
    {
      control_Movie ( nextLattice );
    }
  else
    {
      m_time = -1;
    }

    */

}

void GameOfLife::red ( int **lattice, int x, int y, int color )
{

  int r[7][17] =
  {
    {1,1,1,0,0,0,1,1,1,1,1,0,1,1,1,0,0},
    {1,0,0,1,0,0,1,0,0,0,0,0,1,0,0,1,0},
    {1,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,1},
    {1,1,1,0,0,0,1,1,1,1,0,0,1,0,0,0,1},
    {1,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1},
    {1,0,0,1,0,0,1,0,0,0,0,0,1,0,0,1,0},
    {1,0,0,0,1,0,1,1,1,1,1,0,1,1,1,0,0}
  };

  for ( int i {0}; i<7; ++i )
    {
      for ( int j {0}; j<17; ++j )
        {
          if ( r[i][j] )
            {
              lattice[y+i][x+j] = r[i][j]*color;
            }
        }

    }
}

void GameOfLife::green ( int **lattice, int x, int y, int color )
{

  int r[7][29] =
  {
    {0,1,1,1,0,0,1,1,1,0,0,0,1,1,1,1,1,0,1,1,1,1,1,0,1,0,0,0,1},
    {1,0,0,0,1,0,1,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,1,0,0,1},
    {1,0,0,0,0,0,1,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,1,1,0,1},
    {1,0,1,1,1,0,1,1,1,0,0,0,1,1,1,1,0,0,1,1,1,1,0,0,1,0,1,0,1},
    {1,0,0,0,1,0,1,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,1,1,1},
    {1,0,0,0,1,0,1,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,1,1},
    {0,1,1,1,0,0,1,0,0,0,1,0,1,1,1,1,1,0,1,1,1,1,1,0,1,0,0,0,1}
  };

  for ( int i {0}; i<7; ++i )
    {
      for ( int j {0}; j<29; ++j )
        {

          if ( r[i][j] )
            {
              lattice[y+i][x+j] = r[i][j]*color;
            }
        }

    }
}

void GameOfLife::blue ( int **lattice, int x, int y, int color )
{

  int r[7][21] =
  {
    {1,1,1,0,0,1,0,0,0,0,1,0,0,0,1,0,1,1,1,1,1},
    {1,0,0,1,0,1,0,0,0,0,1,0,0,0,1,0,1,0,0,0,0},
    {1,0,0,1,0,1,0,0,0,0,1,0,0,0,1,0,1,0,0,0,0},
    {1,1,1,0,0,1,0,0,0,0,1,0,0,0,1,0,1,1,1,1,0},
    {1,0,0,1,0,1,0,0,0,0,1,0,0,0,1,0,1,0,0,0,0},
    {1,0,0,1,0,1,0,0,0,0,1,0,0,0,1,0,1,0,0,0,0},
    {1,1,1,0,0,1,1,1,1,0,0,1,1,1,0,0,1,1,1,1,1}

  };

  for ( int i {0}; i<7; ++i )
    {
      for ( int j {0}; j<21; ++j )
        {
          if ( r[i][j] )
            {
              lattice[y+i][x+j] = r[i][j]*color;
            }
        }

    }
}

void GameOfLife::glider ( int **lattice, int x, int y )
{

  lattice[y+0][x+2] = 1;
  lattice[y+1][x+1] = 1;
  lattice[y+2][x+1] = 1;
  lattice[y+2][x+2] = 1;
  lattice[y+2][x+3] = 1;

}

void GameOfLife::house ( int **lattice, int x, int y )
{

  lattice[y+0][x+3] = 1;

  lattice[y+1][x+2] = 1;
  lattice[y+1][x+4] = 1;

  lattice[y+2][x+1] = 1;
  lattice[y+2][x+5] = 1;

  lattice[y+3][x+0] = 1;
  lattice[y+3][x+6] = 1;

  lattice[y+4][x+0] = 1;
  lattice[y+4][x+6] = 1;

  lattice[y+5][x+0] = 1;
  lattice[y+5][x+6] = 1;

  lattice[y+6][x+0] = 1;
  lattice[y+6][x+6] = 1;

  lattice[y+7][x+0] = 1;
  lattice[y+7][x+6] = 1;

  lattice[y+8][x+0] = 1;
  lattice[y+8][x+1] = 1;
  lattice[y+8][x+2] = 1;
  lattice[y+8][x+3] = 1;
  lattice[y+8][x+4] = 1;
  lattice[y+8][x+5] = 1;
  lattice[y+8][x+6] = 1;
}

void GameOfLife::man ( int **lattice, int x, int y )
{

  lattice[y+0][x+1] = 1;

  lattice[y+1][x+0] = 1;
  lattice[y+1][x+1] = 1;
  lattice[y+1][x+2] = 1;

  lattice[y+2][x+1] = 1;

  lattice[y+3][x+0] = 1;
  lattice[y+3][x+2] = 1;

}

void GameOfLife::car ( int **lattice, int x, int y )
{

  lattice[y+0][x+1] = 1;
  lattice[y+0][x+2] = 1;
  lattice[y+0][x+3] = 1;

  lattice[y+1][x+0] = 1;
  lattice[y+1][x+1] = 1;
  lattice[y+1][x+2] = 1;
  lattice[y+1][x+3] = 1;
  lattice[y+1][x+4] = 1;

  lattice[y+2][x+1] = 1;
  lattice[y+2][x+3] = 1;

}

int GameOfLife::getW() const
{
  return m_w;
}
int GameOfLife::getH() const
{
  return m_h;
}
long GameOfLife::getT() const
{
  return m_time;
}













