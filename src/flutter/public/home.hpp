#ifndef FLUTTER_HOME_H
#define FLUTTER_HOME_H

void* FlutterHome_index() {
	srand((unsigned)time(NULL));
	char r = rand()%256;

	Json::Value object;
	Json::Value data;
	Json::Value slides;
	Json::Value cate;
	Json::Value	ad;
	Json::Value shopInfo;
	Json::Value recommend;
	Json::Value item;

	// slides for slides
	item["image"] = "http://p1.music.126.net/AOEDgzKOcgWNsUC34W0OHg==/109951163952526139.jpg?param=640y300";
	item["random"] = r;
	slides.append(item);
	r = rand()%256;
	item["image"] = "http://p1.music.126.net/o2qzE0Is5Qh1pfSKQiVGkA==/18646617697368402.jpg?param=640y300";
	item["random"] = r;
	slides.append(item);
	r = rand()%256;
	item["image"] = "http://p2.music.126.net/nILBk4DaE3yV__25uq-5GQ==/18641120139241412.jpg?param=640y300";
	item["random"] = r;
	slides.append(item);
	r = rand()%256;
	item["image"] = "http://p1.music.126.net/sdow6GiZrSc59WuFbw6rVA==/164926744183505.jpg?param=640y300";
	item["random"] = r;
	slides.append(item);
	
	// clean reuse item
	item.removeMember("random");
	// cate for cate
	// cate no1
	item["image"] = "http://p1.music.126.net/p9U80ex1B1ciPFa125xV5A==/5931865232210340.jpg?param=62y62";
	item["cateName"] = "cate1";
	cate.append(item);

	item["image"] = "http://p1.music.126.net/p9U80ex1B1ciPFa125xV5A==/5931865232210340.jpg?param=62y62";
	item["cateName"] = "cate2";
	cate.append(item);

	item["image"] = "http://p1.music.126.net/p9U80ex1B1ciPFa125xV5A==/5931865232210340.jpg?param=62y62";
	item["cateName"] = "cate3";
	cate.append(item);

	item["image"] = "http://p1.music.126.net/p9U80ex1B1ciPFa125xV5A==/5931865232210340.jpg?param=62y62";
	item["cateName"] = "cate4";
	cate.append(item);

	item["image"] = "http://p1.music.126.net/p9U80ex1B1ciPFa125xV5A==/5931865232210340.jpg?param=62y62";
	item["cateName"] = "cate5";
	cate.append(item);

	// cate no2
	item["image"] = "http://p1.music.126.net/1EN_iqQWU_E3DafzEOh3cA==/3302932937408956.jpg?param=62y62";
	item["cateName"] = "cate6";
	cate.append(item);

	item["image"] = "http://p1.music.126.net/1EN_iqQWU_E3DafzEOh3cA==/3302932937408956.jpg?param=62y62";
	item["cateName"] = "cate7";
	cate.append(item);

	item["image"] = "http://p1.music.126.net/1EN_iqQWU_E3DafzEOh3cA==/3302932937408956.jpg?param=62y62";
	item["cateName"] = "cate8";
	cate.append(item);

	item["image"] = "http://p1.music.126.net/1EN_iqQWU_E3DafzEOh3cA==/3302932937408956.jpg?param=62y62";
	item["cateName"] = "cate9";
	cate.append(item);

	item["image"] = "http://p1.music.126.net/1EN_iqQWU_E3DafzEOh3cA==/3302932937408956.jpg?param=62y62";
	item["cateName"] = "cate10";
	cate.append(item);

	// ad's pic setting
	ad["pic"]		= "https://i0.hdslb.com/bfs/sycp/creative_img/201910/2ee11ba2cdb28b624b51c7f49b4ba7d6.jpg";	

	// shopInfo's setting
	shopInfo["callPic"] = "https://i0.hdslb.com/bfs/bangumi/image/e5e41166cd7eabaa1dde157f4e98473571a56990.jpg@2320w_664h.jpg";
	shopInfo["callNum"] = "15902069589";

	// recomend setting
	// reset item
	item.removeMember("image");
	item.removeMember("cateName");
	item["price"]	= "123";
	item["image"]	= "http://p4.music.126.net/N2HO5xfYEqyQ8q6oxCw8IQ==/18713687906568048.jpg?param=100y100";
	item["mailprice"]="321";
	recommend.append(item);
	recommend.append(item);
	recommend.append(item);
	recommend.append(item);
	recommend.append(item);

	data["slides"]	= slides;
	data["cate"]	= cate;
	data["ad"]		= ad;
	data["shopInfo"]= shopInfo;
	data["recommend"]=recommend;
	object["data"]	= data;

	char* ret;

	std::string content = object.toStyledString();
	ret = new char[content.length() + 1];
	strcpy(ret, content.c_str());

	return ret;
}

#endif
