#ifndef FLUTTER_HOME_H
#define FLUTTER_HOME_H

void* FlutterHome_index() {
	srand((unsigned)time(NULL));
	char r = rand()%256;

	Json::Value data1;
	Json::Value data2;
	Json::Value data3;
	Json::Value item;
	item["image"] = "http://p1.music.126.net/AOEDgzKOcgWNsUC34W0OHg==/109951163952526139.jpg?param=640y300";
	item["random"] = r;
	data3.append(item);
	r = rand()%256;
	item["image"] = "http://p1.music.126.net/o2qzE0Is5Qh1pfSKQiVGkA==/18646617697368402.jpg?param=640y300";
	item["random"] = r;
	data3.append(item);
	r = rand()%256;
	item["image"] = "http://p2.music.126.net/nILBk4DaE3yV__25uq-5GQ==/18641120139241412.jpg?param=640y300";
	item["random"] = r;
	data3.append(item);
	r = rand()%256;
	item["image"] = "http://p1.music.126.net/sdow6GiZrSc59WuFbw6rVA==/164926744183505.jpg?param=640y300";
	item["random"] = r;
	data3.append(item);
	data2["slides"] = data3;
	data1["data"] = data2;

	char* ret;

	std::string content = data1.toStyledString();
	ret = new char[content.length() + 1];
	strcpy(ret, content.c_str());

	return ret;
}

#endif
