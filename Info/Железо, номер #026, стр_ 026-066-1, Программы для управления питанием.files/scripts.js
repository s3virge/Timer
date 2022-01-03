document.domain = 'xakep.ru'

function switchColor (sColor, sObj){
	eval('document.getElementById("' + sObj + '").style.backgroundColor="' + sColor + '"');
}

function onover_image( image ){
	document.getElementById( image ).src = "http://www.gameland.ru/i/i/icons/" + image + "_over.gif";
}
function onout_image( image ){
	document.getElementById( image ).src = "http://www.gameland.ru/i/i/icons/" + image + ".gif";
}

function adjustFrame (frame) {
  if (document.all) {
    var w = frame.document.body.scrollWidth;
    var h = frame.document.body.scrollHeight;
    document.all[frame.name].width = w;
    document.all[frame.name].height = h;
  }
  else if (document.getElementById) {
    var w = frame.document.width;
    var h = frame.document.height;
    document.getElementById(frame.name).width = w;
    document.getElementById(frame.name).height = h;
  }
}

function manualSwitcher(){
	if(eval('document.getElementById("div1").style.display') != 'none')
	{
		divVal = 1;
	}
	
	if(eval('document.getElementById("div2").style.display') != 'none')
	{
		divVal = 2;
	}
	
	if(eval('document.getElementById("div3").style.display') != 'none')
	{
		divVal = 3;	
	}
	switch (divVal){
	 case 1:
		eval('document.getElementById("div2").style.display="block"');
		eval('document.getElementById("div1").style.display="none"');
	break;
	 case 2:
		eval('document.getElementById("div3").style.display="block"');
		eval('document.getElementById("div2").style.display="none"');
	break;
	 case 3:
		eval('document.getElementById("div1").style.display="block"');
		eval('document.getElementById("div3").style.display="none"');	
	break;
	}
}

function selectDivBlock() {
	eval('document.getElementById("div4").style.display="none"');
	var numVal = Math.round(Math.random()) + Math.round(Math.random()) + Math.round(Math.random());
	if(numVal == 0){
		numVal = 3;
	}
	switch (numVal){
	 case 1:
		eval('document.getElementById("div1").style.display="block"');
	break;
	 case 2:
		eval('document.getElementById("div2").style.display="block"');	
	break;
	 case 3:
		eval('document.getElementById("div3").style.display="block"');	
	break;
	}
}

function showArchive(objArch){
	eval('document.getElementById("div4").style.display="none"');
	eval('document.getElementById("div1").style.display="none"');
	eval('document.getElementById("div2").style.display="none"');
	eval('document.getElementById("div3").style.display="none"');
	eval('document.getElementById("'+objArch+'").style.display="block"');
}

function menuBGchange(obj){
	obj.style.backgroundImage="url('http://www.xakep.ru/i/img_new/menu_hover.gif')";
	obj.style.cursor="hand";
	obj.firstChild.nextSibling.style.color="#FFFFFF";
}

function menuBGchangeback(obj){
	obj.style.backgroundImage="url('http://www.xakep.ru/i/img_new/menu.gif')";
	obj.firstChild.nextSibling.style.color="#4D4D4D";
}


function anonsMenuItems(obj, flagNum){
	tagTD1 = obj.firstChild;
	tagTD2 = tagTD1.nextSibling;
	tagImg = tagTD1.firstChild
	tagA = tagTD2.firstChild
	imType = tagImg.src.substring(tagImg.src.length-4, tagImg.src.length);
	if (flagNum == 1) {
	imPathBefore = tagImg.src.substring(0, tagImg.src.length-4);
	tagA.style.color = "gray";
	tagImg.src=imPathBefore+"_hover"+imType;
	}else{
	imPathAfter = tagImg.src.substring(0, tagImg.src.length-10);
	tagA.style.color = "white";
	tagImg.src=imPathAfter+imType;
	}
}



var flag = 1;
var imgFlag = 1;

function openMenu (obj, imgPath){
		elementDIV = obj.lastChild;
		elementIMG = obj.firstChild.nextSibling;

	topCoord = getDivY(obj);
	leftCoord = getDivX(obj);

	elementDIV.style.top = topCoord + 20;
	elementDIV.style.left = leftCoord;
		
	if (flag == 1){
		elementDIV.style.display = 'block';
		elementIMG.src = imgPath + "up.gif";
		flag = 0;
	}else{
		elementDIV.style.display = 'none';
		elementIMG.src = imgPath + "down.gif";
		flag = 1;
	}
}

function getDivX(divElem) {
  xPos = eval(divElem).offsetLeft;
  tempEl = eval(divElem).offsetParent;
    while (tempEl != null) {
     xPos += tempEl.offsetLeft;
     tempEl = tempEl.offsetParent;
    }
  return xPos;
}

function getDivY(divElem) {
   yPos = eval(divElem).offsetTop;
   tempEl = eval(divElem).offsetParent;
     while (tempEl != null) {
      yPos += tempEl.offsetTop;
      tempEl = tempEl.offsetParent;
     }
   return yPos;
}

function altMenuBGchange(obj){
	if (imgFlag == 1){
	obj.style.cursor="hand";
	obj.style.backgroundImage="url('http://www.xakep.ru/i/img_new/menu_hover.gif')";
	imgFlag = 0;
	
	}else{
	obj.style.backgroundImage="url('http://www.xakep.ru/i/img_new/menu.gif')";
	imgFlag = 1;
	}
}
