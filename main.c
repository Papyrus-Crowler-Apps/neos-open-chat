#include <lvmx.h>
#include <jsmn.h>


int* textbuff = 100000;
int* textbuff2 = 200000;
int endpoint[] = "https://neos.gammalab.net/NeosOpenChat";

void setRGB(int slot, float R, float G, float B) {
	setDVFloat(slot, "R", R);
	setDVFloat(slot, "G", G);
	setDVFloat(slot, "B", B);
}

int objAccess(int* json, jsmntok_t* t, int objitr, int* key) {
	int i;
	int jsonitr = objitr + 1;
	int pos = -1;
	int size = t[objitr].size;
	for (i = 0; i < size; i++) {

		json[t[jsonitr].end] = 0;

		if (strcmp(key, &json[t[jsonitr].start])) {
			pos = jsonitr + 1;
			break;
		}

		jsonitr += t[jsonitr].size + 1;
	}
	return pos;
}

int getString(int* json, jsmntok_t* t, int id, int* dest) {
	if (t[id].type != JSMN_STRING) {
		debuglog("String expected\n");
		return -1;
	}

	json[t[id].end] = 0;
	strcpy(dest, &json[t[id].start]);

	return 0;
}

int calcJsonSize(jsmntok_t* t, int itr) {
	int size = t[itr++].size;
	int sum = 0;
	int counter = 0;

	for (; counter < size; itr++) {
		int type = t[itr].type;
		if (type == JSMN_OBJECT) {
			size += t[itr].size - 1;
		}else if (type == JSMN_ARRAY) {
			size += t[itr].size - 1;
		} else {
			if (t[itr].size == 0) counter++;
		}
		sum++;
	}

	return sum+1;
}


int main() {


	int username[64];

	getDVString(CURRENTAPP_SLOT, "username", username);
	if ( strcmp(username, "") ) {
		getDVString(DEVICEROOT_SLOT, "OWNER", username);
		setDVString(CURRENTAPP_SLOT, "username", username);
	}

	// レイアウト
	int headerRect = createSlotFromTemplate("UIXempty");
	setDVFloat(headerRect, "Amy", 0.95f);
	setSlotParent(headerRect, UIXROOT_SLOT);

	int headerImage = createSlotFromTemplate("UIXimage");
	setRGB(headerImage, 0.0157f, 0.6784f, 0.7490f);
	setSlotParent(headerImage, headerRect);

	int threadInput = createSlotFromTemplate("UIXtextField");
	setDVString(threadInput, "content", "main");
	setSlotParent(threadInput, headerImage);

	int bodyRect = createSlotFromTemplate("UIXempty");
	setDVFloat(bodyRect, "Amy", 0.15f);
	setDVFloat(bodyRect, "AMy", 0.95f);
	setSlotParent(bodyRect, UIXROOT_SLOT);

	int bodyMask = createSlotFromTemplate("UIXmask");
	setRGB(bodyMask, 0.0f, 0.11f, 0.25f);
	setSlotParent(bodyMask, bodyRect);

	int bodyScroll = createSlotFromTemplate("UIXscrollV");
	setSlotParent(bodyScroll, bodyMask);

	int body = createSlotFromTemplate("UIXlayoutV");
	setDVInt(body, "expandH", 0);
	setSlotParent(body, bodyScroll);

	int userInputRect = createSlotFromTemplate("UIXempty");
	setDVFloat(userInputRect, "AMy", 0.15f);
	setDVFloat(userInputRect, "Amy", 0.1f);
	setSlotParent(userInputRect, UIXROOT_SLOT);

	int userImage = createSlotFromTemplate("UIXimage");
	setRGB(userImage, 0.012f, 0.396f, 0.549f);
	setSlotParent(userImage, userInputRect);

	int userInput = createSlotFromTemplate("UIXtextField");
	setDVFloat(userInput, "size", 20.0f);
	setDVString(userInput, "content", username);
	setSlotParent(userInput, userImage);


	int textInputRect = createSlotFromTemplate("UIXempty");
	setDVFloat(textInputRect, "AMx", 0.8f);
	setDVFloat(textInputRect, "AMy", 0.1f);
	setSlotParent(textInputRect, UIXROOT_SLOT);

	int textInput = createSlotFromTemplate("UIXtextField");
	setDVFloat(textInput, "size", 20.0f);
	setSlotParent(textInput, textInputRect);

	int submitButtonRect = createSlotFromTemplate("UIXempty");
	setDVFloat(submitButtonRect, "Amx", 0.8f);
	setDVFloat(submitButtonRect, "AMy", 0.1f);
	setSlotParent(submitButtonRect, UIXROOT_SLOT);
	int submitImage = createSlotFromTemplate("UIXimage");
	setRGB(submitImage, 0.0157f, 0.6784f, 0.7490f);
	setSlotParent(submitImage, submitButtonRect);
	int submitButton = createSlotFromTemplate("UIXbutton");
	setSlotParent(submitButton, submitImage);
	int submitText = createSlotFromTemplate("UIXtext");
	setDVFloat(submitText, "size", 20.0f);
	setDVString(submitText, "content", "submit");
	setRGB(submitText, 0.02f, 0.949f, 0.949f);
	setSlotParent(submitText, submitButton);

	int messageRect = createSlotFromTemplate("UIXlayoutElem");
	setDVFloat(messageRect, "minH", 64.0f);

	int infoRect = createSlotFromTemplate("UIXempty");
	setDVFloat(infoRect, "Amy", 0.5f);
	setSlotParent(infoRect, messageRect);
	int usernameText = createSlotFromTemplate("UIXtext");
	setRGB(usernameText, 0.0157f, 0.6784f, 0.7490f);
	setDVFloat(usernameText, "size", 20.0f);
	setDVInt(usernameText, "alignH", 0);
	setSlotParent(usernameText, infoRect);
	int dateText = createSlotFromTemplate("UIXtext");
	setRGB(dateText, 0.0157f, 0.6784f, 0.7490f);
	setDVFloat(dateText, "size", 20.0f);
	setDVInt(dateText, "alignH", 2);
	setSlotParent(dateText, infoRect);
	int contentRect = createSlotFromTemplate("UIXempty");
	setDVFloat(contentRect, "AMy", 0.5f);
	setSlotParent(contentRect, messageRect);
	int contentText = createSlotFromTemplate("UIXtext");
	setDVFloat(contentText, "size", 20.0f);
	setDVInt(contentText, "alignH", 0);
	setDVInt(contentText, "alignV", 0);
	setSlotParent(contentText, contentRect);

	int httpGET = createSlotFromTemplate("httpGET");
	int httpPOST = createSlotFromTemplate("httpPOST");
	setDVString(httpPOST, "URL", endpoint);
	setDVString(httpPOST, "mediaType", "application/json");


	int threadname[32];
	int content[256];
	int date[32];

	int getting = 0;
	int reload = 1;

	jsmn_parser p;
	jsmntok_t t[512];
	jsmn_init(&p);

	while (1) {

		// スレッド名の設定
		if ((getDVInt(threadInput, "editFinished") != 0) || (reload != 0)) {
			reload = 0;
			setDVInt(threadInput, "editFinished", 0);
			getDVString(threadInput, "content", threadname);
			format(textbuff, "%s?thread=%s", endpoint, threadname);
			debuglog("get:");
			debuglog(textbuff);
			debuglog("\n");

			setDVString(httpGET, "URL", textbuff);
			sendDI(httpGET, "send");

			getting = 1;
		}

		// チャットデータ受信イベント
		if ((getting == 1) && (getDVInt(httpGET, "status")) == 2) {
			getting = 0;
			setDVInt(httpGET, "status", 0);
			getDVString(httpGET, "result", textbuff);

			debuglog(textbuff);
			debuglog("\n");

			jsmn_init(&p);
			setDVInt(DEVICEROOT_SLOT, "OVERCLOCK", 512);
			int r = jsmn_parse(&p, textbuff, strlen(textbuff), t, sizeof(t));
			setDVInt(DEVICEROOT_SLOT, "OVERCLOCK", 30);
			
			if (r < 0) {
				debuglog("JSON parse error\n");
				debuglog(textbuff);
				debuglog("\nerrorcode: ");
				itos(r, textbuff);
				debuglog(textbuff);
				debuglog("\nsizeof(t): ");
				itos(sizeof(t), textbuff);
				debuglog(textbuff);
				return -1;
			}

			if (t[0].type != JSMN_ARRAY) {
				debuglog("ARRAY expected\n");
				debuglog(textbuff);
				debuglog("\n");
				itos(t[0].type, textbuff);
				debuglog(textbuff);
				return -1;
			}


			format(textbuff2, "r: %d\n", r);
			debuglog(textbuff2);
			format(textbuff2, "size: %d\n", t[0].size);
			debuglog(textbuff2);

			int size = t[0].size;

			int i;
			int pos = 1;

			__raw(float, "DESS", 1, body);

			for (i = 0; i < size; i++) {
				int strid = objAccess(textbuff, t, pos, "Username");
				getString(textbuff, t, strid, username);
				setDVString(usernameText, "content", username);

				strid = objAccess(textbuff, t, pos, "Date");
				getString(textbuff, t, strid, date);
				setDVString(dateText, "content", date);

				strid = objAccess(textbuff, t, pos, "Content");
				getString(textbuff, t, strid, content);
				setDVString(contentText, "content", content);

				int newcard = dupSlot(messageRect);
				setSlotParent(newcard, body);

				pos += calcJsonSize(t, pos);
			}
		}

		// メッセージの送信
		if (getDVInt(submitButton, "pressed") != 0) {
			setDVInt(submitButton, "pressed", 0);
			getDVString(textInput, "content", content);

			format(textbuff, "{\"thread\": \"%s\", \"username\": \"%s\", \"content\": \"%s\"}", threadname, username, content);
			setDVString(httpPOST, "content", textbuff);
			sendDI(httpPOST, "send");
			debuglog("post:");
			debuglog(textbuff);
			debuglog("\n");

			setDVString(textInput, "content", "");

			reload = 1;
		}

		if (getDVInt(userInput, "editFinished") != 0) {
			setDVInt(userInput, "editFinished", 0);
			getDVString(userInput, "content", username);
			setDVString(CURRENTAPP_SLOT, "username", username);
		}


	}

	return -1;

}
