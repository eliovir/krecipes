/***************************************************************************
 *   Copyright (C) 2003                                                    *
 *                                                                         *
 *   Unai Garro (ugarro@users.sourceforge.net)                             *
 *   Cyril Bosselut (bosselut@b1project.com)                               *
 *   Jason Kivlighn (mizunoami44@users.sourceforge.net)                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <klocale.h>

struct ingredient_data
{
	QString name;
	int usda_id;
};

static ingredient_data ingredient_data_list[] = {
  {I18N_NOOP("active baker's yeast"),18375},
  {I18N_NOOP("all purpose flour"),20081},
  {I18N_NOOP("all-purpose flour"),20081},
  {I18N_NOOP("allspice"),2001},
  {I18N_NOOP("almond extract"),12071},
  {I18N_NOOP("almonds"),12061},
  {I18N_NOOP("apple juice"),9016},
  {I18N_NOOP("apple slices"),9008},
  {I18N_NOOP("apples"),9003},
  {I18N_NOOP("applesauce"),9402},
  {I18N_NOOP("applesauce, unsweetened"),9019},
  {I18N_NOOP("apricot jam"),19719},
  {I18N_NOOP("bacon"),10123},
  {I18N_NOOP("baking potato"),11362},
  {I18N_NOOP("baking powder"),18369},
  {I18N_NOOP("baking soda"),18372},
  {I18N_NOOP("bananas"),9040},
  {I18N_NOOP("bananas, whole"),9040},
  {I18N_NOOP("barley"),20004},
  {I18N_NOOP("basil"),2003},
  {I18N_NOOP("basil (fresh)"),2044},
  {I18N_NOOP("bay leaf"),2004},
  {I18N_NOOP("bean sprouts"),11248},
  {I18N_NOOP("beans, black"),16014},
  {I18N_NOOP("beef bottom round"),13399},
  {I18N_NOOP("beef bouillon"),6075},
  {I18N_NOOP("beef broth"),6008},
  {I18N_NOOP("beef stock"),6476},
  {I18N_NOOP("beef, brisket"),13022},
  {I18N_NOOP("beer"),14003},
  {I18N_NOOP("bell peppers (red, green, yellow)"),11333},
  {I18N_NOOP("black pepper"),2030},
  {I18N_NOOP("boiling water"),14429},
  {I18N_NOOP("bologna"),7008},
  {I18N_NOOP("bourbon"),14551},
  {I18N_NOOP("bran flakes"),8153},
  {I18N_NOOP("bread"),18069},
  {I18N_NOOP("bread crumbs"),18079},
  {I18N_NOOP("bread cubes"),18079},
  {I18N_NOOP("broccoli"),11090},
  {I18N_NOOP("broccoli (frozen-thawed)"),11092},
  {I18N_NOOP("broccoli spears, frozen"),11094},
  {I18N_NOOP("broccoli, frozen"),11092},
  {I18N_NOOP("brown rice"),20040},
  {I18N_NOOP("brown sugar"),19334},
  {I18N_NOOP("bulgur"),20012},
  {I18N_NOOP("butter"),1145},
  {I18N_NOOP("butter (1/2 stick)"),1145},
  {I18N_NOOP("butter or margarine"),1145},
  {I18N_NOOP("buttermilk"),1088},
  {I18N_NOOP("cabbage"),11109},
  {I18N_NOOP("canned apple slice"),19312},
  {I18N_NOOP("canned apple slices"),19312},
  {I18N_NOOP("canned beans"),16006},
  {I18N_NOOP("canned beef"),13934},
  {I18N_NOOP("canned black beans"),16018},
  {I18N_NOOP("canned carrot slices"),11128},
  {I18N_NOOP("canned cooked squid"),15175},
  {I18N_NOOP("canned corn"),11174},
  {I18N_NOOP("canned crushed tomatoes"),11693},
  {I18N_NOOP("canned green beans"),11056},
  {I18N_NOOP("canned green peas"),11308},
  {I18N_NOOP("canned jalapeno pepper"),11632},
  {I18N_NOOP("canned kidney beans"),16029},
  {I18N_NOOP("canned pinto beans"),16044},
  {I18N_NOOP("canned potato"),11376},
  {I18N_NOOP("canned red pepper"),2031},
  {I18N_NOOP("canned red tart cherries in water"),9065},
  {I18N_NOOP("canned sweet potatoes"),11514},
  {I18N_NOOP("canned tomatoes"),11693},
  {I18N_NOOP("canned wax beans"),16029},
  {I18N_NOOP("canned white beans"),16051},
  {I18N_NOOP("canned whole kernel corn"),11771},
  {I18N_NOOP("carrot"),11124},
  {I18N_NOOP("carrot slices, canned"),11128},
  {I18N_NOOP("carrots"),11124},
  {I18N_NOOP("carrots, frozen"),11130},
  {I18N_NOOP("carrots, peeled and chopped"),11124},
  {I18N_NOOP("catsup"),11935},
  {I18N_NOOP("cauliflower, frozen"),11137},
  {I18N_NOOP("cayenne"),2031},
  {I18N_NOOP("celery"),11143},
  {I18N_NOOP("celery leaf"),11143},
  {I18N_NOOP("celery salt"),2007},
  {I18N_NOOP("celery seed"),2007},
  {I18N_NOOP("cheddar cheese"),1009},
  {I18N_NOOP("cheese"),1009},
  {I18N_NOOP("cherry pie filling"),19314},
  {I18N_NOOP("chicken"),5011},
  {I18N_NOOP("chicken (1-2 lb)"),5011},
  {I18N_NOOP("chicken bouillon"),6081},
  {I18N_NOOP("chicken breast"),5062},
  {I18N_NOOP("chicken broth"),6013},
  {I18N_NOOP("chicken pieces"),5008},
  {I18N_NOOP("chicken stock"),6413},
  {I18N_NOOP("chicken thigh"),5080},
  {I18N_NOOP("chicken, cooked"),5011},
  {I18N_NOOP("chickens, whole"),5123},
  {I18N_NOOP("chili"),16059},
  {I18N_NOOP("chili powder"),2009},
  {I18N_NOOP("chinese turnip"),11564},
  {I18N_NOOP("chives"),11615},
  {I18N_NOOP("chocolate chip"),19139},
  {I18N_NOOP("chocolate pudding mix (instant)"),19123},
  {I18N_NOOP("chopped capers"),2054},
  {I18N_NOOP("chopped carrot"),11124},
  {I18N_NOOP("chopped celery"),11143},
  {I18N_NOOP("chopped chives"),11156},
  {I18N_NOOP("chopped green chilies"),11980},
  {I18N_NOOP("chopped onion"),11282},
  {I18N_NOOP("chopped onions"),11282},
  {I18N_NOOP("chopped onions, frozen"),11287},
  {I18N_NOOP("chopped parsley"),11297},
  {I18N_NOOP("chopped pimientos"),11943},
  {I18N_NOOP("chopped walnuts"),12154},
  {I18N_NOOP("cider vinegar"),2048},
  {I18N_NOOP("cilantro"),11971},
  {I18N_NOOP("cinnamon"),2010},
  {I18N_NOOP("clear honey"),19296},
  {I18N_NOOP("cloves"),2011},
  {I18N_NOOP("cloves garlic"),11215},
  {I18N_NOOP("cloves or allspice"),2011},
  {I18N_NOOP("cocoa"),19165},
  {I18N_NOOP("coconut"),12104},
  {I18N_NOOP("coconut milk (canned)"),12118},
  {I18N_NOOP("cold butter"),1145},
  {I18N_NOOP("cold water"),14429},
  {I18N_NOOP("cold water to mix"),14429},
  {I18N_NOOP("condensed milk (sweetened)"),1095},
  {I18N_NOOP("cooked chicken"),5124},
  {I18N_NOOP("cooked chicken breast"),5065},
  {I18N_NOOP("cooked ham"),10151},
  {I18N_NOOP("cooked turkey"),5182},
  {I18N_NOOP("cooked wild rice"),20089},
  {I18N_NOOP("cookie crust (chocolate - 8-9 in)"),18398},
  {I18N_NOOP("Cool Whip"),1054},
  {I18N_NOOP("corn chips"),19003},
  {I18N_NOOP("corn meal"),20020},
  {I18N_NOOP("corn oil"),4518},
  {I18N_NOOP("corn starch"),20027},
  {I18N_NOOP("corn syrup"),19350},
  {I18N_NOOP("cornmeal"),20022},
  {I18N_NOOP("cornstarch"),20027},
  {I18N_NOOP("cottage cheese"),1015},
  {I18N_NOOP("cream (heavy)"),1053},
  {I18N_NOOP("cream cheese"),1017},
  {I18N_NOOP("cream of celery soup"),6479},
  {I18N_NOOP("cream of chicken soup"),6483},
  {I18N_NOOP("cream of chicken soup, condensed"),6016},
  {I18N_NOOP("cream of mushroom soup"),6443},
  {I18N_NOOP("cream of mushroom soup, condensed"),6043},
  {I18N_NOOP("croutons to serve"),18243},
  {I18N_NOOP("crushed pineapple in juice"),9268},
  {I18N_NOOP("cucumber"),11205},
  {I18N_NOOP("cucumber, whole"),11206},
  {I18N_NOOP("cucumbers"),11206},
  {I18N_NOOP("cucumbers, whole"),11206},
  {I18N_NOOP("cumin"),2014},
  {I18N_NOOP("currants"),9084},
  {I18N_NOOP("dehydrated onion flakes"),11284},
  {I18N_NOOP("diced chicken"),5039},
  {I18N_NOOP("dried basil"),2044},
  {I18N_NOOP("dried onion"),11284},
  {I18N_NOOP("dried oregano"),2027},
  {I18N_NOOP("dried parsley"),2029},
  {I18N_NOOP("dried porcini mushrooms"),11268},
  {I18N_NOOP("dried red pepper"),2031},
  {I18N_NOOP("dried sage"),2038},
  {I18N_NOOP("dry bread crumbs"),18079},
  {I18N_NOOP("dry mustard"),2024},
  {I18N_NOOP("dry nonfat milk powder"),1091},
  {I18N_NOOP("egg"),1123},
  {I18N_NOOP("egg noodles (medium)"),20409},
  {I18N_NOOP("egg substitute"),1144},
  {I18N_NOOP("eggplant"),11209},
  {I18N_NOOP("eggplant (baby)"),11209},
  {I18N_NOOP("eggs"),1134},
  {I18N_NOOP("eggs, hard cooked"),1129},
  {I18N_NOOP("eggs, hard-boiled"),1129},
  {I18N_NOOP("egg whites"),1124},
  {I18N_NOOP("egg yolk"),1125},
  {I18N_NOOP("egg yolks"),1125},
  {I18N_NOOP("elbow macaroni"),20099},
  {I18N_NOOP("flaked almonds"),12061},
  {I18N_NOOP("flat anchovies"),15001},
  {I18N_NOOP("flour"),20081},
  {I18N_NOOP("flour tortillas"),18364},
  {I18N_NOOP("flour, all-purpose"),20081},
  {I18N_NOOP("flour, white"),20581},
  {I18N_NOOP("flour, whole-grain wheat"),20080},
  {I18N_NOOP("fresh spinach"),11457},
  {I18N_NOOP("freshly chopped coriander"),11165},
  {I18N_NOOP("freshly chopped parsley"),11297},
  {I18N_NOOP("frozen broccoli"),11092},
  {I18N_NOOP("frozen broccoli spears"),11094},
  {I18N_NOOP("frozen cauliflower"),11137},
  {I18N_NOOP("frozen corn"),11179},
  {I18N_NOOP("frozen egg whites"),1172},
  {I18N_NOOP("frozen fish"),15027},
  {I18N_NOOP("frozen green beans"),11060},
  {I18N_NOOP("frozen orange juice concentrate"),9214},
  {I18N_NOOP("frozen peas"),11814},
  {I18N_NOOP("frozen whole egg"),1171},
  {I18N_NOOP("garlic"),11215},
  {I18N_NOOP("garlic clove"),11215},
  {I18N_NOOP("garlic cloves"),11215},
  {I18N_NOOP("garlic powder"),2020},
  {I18N_NOOP("garlic salt"),2020},
  {I18N_NOOP("gelatin"),19173},
  {I18N_NOOP("gelatin (peach-flavored)"),14397},
  {I18N_NOOP("ginger"),2021},
  {I18N_NOOP("ginger, fresh"),2021},
  {I18N_NOOP("graham cracker crust (8 or 9 in)"),18173},
  {I18N_NOOP("granny Smith apple"),9003},
  {I18N_NOOP("granulated garlic"),16125},
  {I18N_NOOP("grapefruit"),9112},
  {I18N_NOOP("gravy"),6116},
  {I18N_NOOP("green beans (fresh)"),11052},
  {I18N_NOOP("green chile"),11980},
  {I18N_NOOP("green chiles"),11670},
  {I18N_NOOP("green chili pepper"),11670},
  {I18N_NOOP("green onion"),11282},
  {I18N_NOOP("green pepper"),11333},
  {I18N_NOOP("green peppers"),11333},
  {I18N_NOOP("ground almonds"),12061},
  {I18N_NOOP("ground beef"),13309},
  {I18N_NOOP("ground chuck"),13302},
  {I18N_NOOP("ground cinnamon"),2010},
  {I18N_NOOP("ground clove"),2011},
  {I18N_NOOP("ground cloves"),2011},
  {I18N_NOOP("ground cumin"),2014},
  {I18N_NOOP("ground ginger"),2021},
  {I18N_NOOP("ground nutmeg"),2025},
  {I18N_NOOP("ground oregano"),2027},
  {I18N_NOOP("ground pepper"),2030},
  {I18N_NOOP("ground thyme"),2042},
  {I18N_NOOP("ham"),7027},
  {I18N_NOOP("hamburger"),13309},
  {I18N_NOOP("hamburger bun"),18641},
  {I18N_NOOP("hamburger buns"),18641},
  {I18N_NOOP("hamburger rolls"),18641},
  {I18N_NOOP("hazelnuts"),12120},
  {I18N_NOOP("heavy cream"),1053},
  {I18N_NOOP("herb stuffing"),18082},
  {I18N_NOOP("honey"),19296},
  {I18N_NOOP("horseradish"),2055},
  {I18N_NOOP("hot pepper flakes"),2031},
  {I18N_NOOP("hot pepper sauce"),6168},
  {I18N_NOOP("hot sauce"),6164},
  {I18N_NOOP("hot water"),14429},
  {I18N_NOOP("instant dry milk"),1155},
  {I18N_NOOP("instant rice"),20048},
  {I18N_NOOP("Italian salad dressing"),4114},
  {I18N_NOOP("jalapeno peppers"),11979},
  {I18N_NOOP("juice of 1 lemon"),9152},
  {I18N_NOOP("ketchup"),11935},
  {I18N_NOOP("lasagna noodles (8 noodles)"),20409},
  {I18N_NOOP("leaf lettuce"),11253},
  {I18N_NOOP("leeks"),11246},
  {I18N_NOOP("lemon"),9150},
  {I18N_NOOP("lemon juice"),9152},
  {I18N_NOOP("lemon juice, bottled"),9153},
  {I18N_NOOP("lemon juice, frozen"),9154},
  {I18N_NOOP("lemon peel"),9156},
  {I18N_NOOP("lemon, sliced"),9151},
  {I18N_NOOP("lentil"),16069},
  {I18N_NOOP("lettuce"),11253},
  {I18N_NOOP("lime juice"),9160},
  {I18N_NOOP("long-grain white rice"),20044},
  {I18N_NOOP("low sodium beef broth"),6475},
  {I18N_NOOP("low sodium chicken broth"),6413},
  {I18N_NOOP("lowfat 1% milk"),1084},
  {I18N_NOOP("lowfat cheddar cheese"),1168},
  {I18N_NOOP("lowfat cottage cheese"),1016},
  {I18N_NOOP("lowfat mayonnaise"),4018},
  {I18N_NOOP("lowfat mozzarella cheese"),1028},
  {I18N_NOOP("lowfat yogurt"),1117},
  {I18N_NOOP("low-sodium chicken broth"),6413},
  {I18N_NOOP("low-sodium soy sauce"),16125},
  {I18N_NOOP("low-sodium vegetable stock"),6468},
  {I18N_NOOP("margarine"),4071},
  {I18N_NOOP("margarine, soft"),4092},
  {I18N_NOOP("marjoram"),2023},
  {I18N_NOOP("marjoram leaves"),2023},
  {I18N_NOOP("mayonnaise"),4025},
  {I18N_NOOP("medium-size egg"),1123},
  {I18N_NOOP("milk"),1081},
  {I18N_NOOP("mint leaves"),2064},
  {I18N_NOOP("mixed fruit in syrup"),9099},
  {I18N_NOOP("mixed vegetables"),11318},
  {I18N_NOOP("mixed vegetables, frozen"),11583},
  {I18N_NOOP("molasses"),19304},
  {I18N_NOOP("molasses, dark"),19305},
  {I18N_NOOP("monterey jack cheese"),1025},
  {I18N_NOOP("mozzarella cheese"),1028},
  {I18N_NOOP("mung beans"),11626},
  {I18N_NOOP("mushroom"),11260},
  {I18N_NOOP("mushrooms"),11260},
  {I18N_NOOP("mustard"),2046},
  {I18N_NOOP("mustard seeds"),2024},
  {I18N_NOOP("navy beans, cooked"),16338},
  {I18N_NOOP("nonfat dry milk"),1092},
  {I18N_NOOP("nonfat milk"),1085},
  {I18N_NOOP("noodles"),20409},
  {I18N_NOOP("noodles (lasagne)"),20409},
  {I18N_NOOP("noodles (lasagne) (6-8 bunches)"),20409},
  {I18N_NOOP("noodles, egg (medium)"),20409},
  {I18N_NOOP("nutmeg"),2025},
  {I18N_NOOP("nuts, chopped"),16087},
  {I18N_NOOP("oil"),4518},
  {I18N_NOOP("oil, peanut"),4042},
  {I18N_NOOP("oil, sesame"),4058},
  {I18N_NOOP("olive oil"),4053},
  {I18N_NOOP("onion"),11282},
  {I18N_NOOP("onion, medium"),11282},
  {I18N_NOOP("onion powder"),2026},
  {I18N_NOOP("onion salt"),2026},
  {I18N_NOOP("onion soup"),6445},
  {I18N_NOOP("onion, chopped"),11282},
  {I18N_NOOP("onion, large"),11282},
  {I18N_NOOP("onions"),11284},
  {I18N_NOOP("onions, chopped"),11282},
  {I18N_NOOP("orange juice"),9206},
  {I18N_NOOP("orange juice, from frozen concentra"),9215},
  {I18N_NOOP("orange rind"),9216},
  {I18N_NOOP("orange zest"),9216},
  {I18N_NOOP("oranges"),9200},
  {I18N_NOOP("oregano"),2027},
  {I18N_NOOP("Oreo Cookies"),18166},
  {I18N_NOOP("paprika"),2028},
  {I18N_NOOP("paprika pepper"),2028},
  {I18N_NOOP("parmesan cheese"),1032},
  {I18N_NOOP("parsley"),2029},
  {I18N_NOOP("parsley flakes"),2029},
  {I18N_NOOP("parsley stalks"),11297},
  {I18N_NOOP("parsnips"),11298},
  {I18N_NOOP("pasta shells"),20100},
  {I18N_NOOP("peach slices in syrup"),9240},
  {I18N_NOOP("peanut butter"),16098},
  {I18N_NOOP("pearled barley"),20005},
  {I18N_NOOP("peas, canned"),11308},
  {I18N_NOOP("peas, frozen"),11312},
  {I18N_NOOP("pecans"),12142},
  {I18N_NOOP("pecans, chopped"),12142},
  {I18N_NOOP("pepper"),2030},
  {I18N_NOOP("Pepperidge Farm stuffing"),18082},
  {I18N_NOOP("pickle relish"),11945},
  {I18N_NOOP("pickles"),11937},
  {I18N_NOOP("pimento"),11943},
  {I18N_NOOP("pine kernels"),12149},
  {I18N_NOOP("pineapple chunks in juice"),9268},
  {I18N_NOOP("pineapple juice"),9273},
  {I18N_NOOP("pinto beans"),16042},
  {I18N_NOOP("pinto beans, canned"),16044},
  {I18N_NOOP("plain flour"),20081},
  {I18N_NOOP("plain lowfat yogurt"),1117},
  {I18N_NOOP("plain low-fat yogurt"),1117},
  {I18N_NOOP("plain yogurt"),1117},
  {I18N_NOOP("pork roast"),10083},
  {I18N_NOOP("potato"),11352},
  {I18N_NOOP("potato flakes"),11379},
  {I18N_NOOP("potato Granules"),11380},
  {I18N_NOOP("potato, canned"),11376},
  {I18N_NOOP("potatoes"),11352},
  {I18N_NOOP("potatoes (red-skinned)"),11352},
  {I18N_NOOP("poultry seasoning"),2034},
  {I18N_NOOP("powdered sugar"),19335},
  {I18N_NOOP("processed American cheese"),1149},
  {I18N_NOOP("provolone cheese"),1035},
  {I18N_NOOP("prunes"),9291},
  {I18N_NOOP("pumpkin"),11422},
  {I18N_NOOP("radishes"),11429},
  {I18N_NOOP("raisins"),9298},
  {I18N_NOOP("red burgundy wine"),14096},
  {I18N_NOOP("red onion"),11282},
  {I18N_NOOP("red pepper"),2031},
  {I18N_NOOP("red snapper fillets"),15101},
  {I18N_NOOP("red wine"),14096},
  {I18N_NOOP("reduced calorie mayonnaise"),4013},
  {I18N_NOOP("relish"),11945},
  {I18N_NOOP("rice"),20045},
  {I18N_NOOP("ricotta cheese"),1037},
  {I18N_NOOP("ripe dessert pears"),9252},
  {I18N_NOOP("rolled oats"),20038},
  {I18N_NOOP("rosemary"),2063},
  {I18N_NOOP("rum flavoring or vanilla"),2050},
  {I18N_NOOP("sage"),2038},
  {I18N_NOOP("salad dressing (Miracle Whip)"),4025},
  {I18N_NOOP("salad onions"),11282},
  {I18N_NOOP("salsa"),6164},
  {I18N_NOOP("salt"),2047},
  {I18N_NOOP("sandwich rolls"),18349},
  {I18N_NOOP("sausage"),7063},
  {I18N_NOOP("scallions"),11291},
  {I18N_NOOP("self-raising flour"),20082},
  {I18N_NOOP("sesame oil"),4058},
  {I18N_NOOP("shallots"),11677},
  {I18N_NOOP("shortening"),4547},
  {I18N_NOOP("shredded carrots"),11124},
  {I18N_NOOP("shredded lettuce"),11252},
  {I18N_NOOP("shrimp (raw, medium-size)"),15149},
  {I18N_NOOP("skim milk"),1085},
  {I18N_NOOP("skinless boneless chicken breast"),5062},
  {I18N_NOOP("sliced carrots"),11124},
  {I18N_NOOP("small mushrooms"),11260},
  {I18N_NOOP("small onions or shallots"),11282},
  {I18N_NOOP("smoked bacon"),10124},
  {I18N_NOOP("soft bread crumbs"),18079},
  {I18N_NOOP("sour cream"),1056},
  {I18N_NOOP("soy sauce"),16125},
  {I18N_NOOP("spaghetti"),20120},
  {I18N_NOOP("spinach leaf"),11457},
  {I18N_NOOP("sprig fresh thyme"),2049},
  {I18N_NOOP("stew beef"),13286},
  {I18N_NOOP("stock"),6413},
  {I18N_NOOP("sugar"),19335},
  {I18N_NOOP("sugar, brown"),19334},
  {I18N_NOOP("sweet pickle"),11940},
  {I18N_NOOP("sweet pickle relish"),11945},
  {I18N_NOOP("sweet potato"),11647},
  {I18N_NOOP("sweet potato, canned"),11645},
  {I18N_NOOP("swiss cheese"),1040},
  {I18N_NOOP("taco sauce"),6168},
  {I18N_NOOP("taco seasoning mix"),2034},
  {I18N_NOOP("taco shells"),18360},
  {I18N_NOOP("taco spice"),2034},
  {I18N_NOOP("thyme"),2042},
  {I18N_NOOP("tomato juice"),11540},
  {I18N_NOOP("tomato paste"),11887},
  {I18N_NOOP("tomato sauce"),11549},
  {I18N_NOOP("tomatoes"),11529},
  {I18N_NOOP("tomatoes, canned"),11533},
  {I18N_NOOP("tomatoes, stewed"),11693},
  {I18N_NOOP("tortilla chips"),19056},
  {I18N_NOOP("tostada shell"),18363},
  {I18N_NOOP("tuna"),15126},
  {I18N_NOOP("tuna in water, canned"),15126},
  {I18N_NOOP("turkey"),7079},
  {I18N_NOOP("turkey ham"),7264},
  {I18N_NOOP("turmeric"),2043},
  {I18N_NOOP("turnip"),11564},
  {I18N_NOOP("unflavored gelatin"),19173},
  {I18N_NOOP("vanilla"),2050},
  {I18N_NOOP("vanilla extract"),2050},
  {I18N_NOOP("vanilla pudding mix (instant)"),19202},
  {I18N_NOOP("veal shank"),17278},
  {I18N_NOOP("vegetable oil"),4518},
  {I18N_NOOP("vegetable stock"),6468},
  {I18N_NOOP("Velveeta"),1149},
  {I18N_NOOP("vinegar"),2048},
  {I18N_NOOP("walnuts"),12154},
  {I18N_NOOP("warm water"),14429},
  {I18N_NOOP("water"),14429},
  {I18N_NOOP("whipped topping"),1054},
  {I18N_NOOP("white flour"),20481},
  {I18N_NOOP("white pepper"),2032},
  {I18N_NOOP("white rice"),20050},
  {I18N_NOOP("white turnips"),11564},
  {I18N_NOOP("white vinegar"),2048},
  {I18N_NOOP("white wine"),14106},
  {I18N_NOOP("whole wheat flour"),20080},
  {I18N_NOOP("yeast"),18375},
  {I18N_NOOP("yellow squash"),11641},
  {I18N_NOOP("zucchini"),11477},
  {I18N_NOOP("zucchini slices"),11953},
  {0,0}
};

