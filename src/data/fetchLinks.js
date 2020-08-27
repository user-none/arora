(function (){
    var links = new Array;
    var it = document.evaluate('/html/head/link', document, null, XPathResult.ANY_TYPE, null);
    var link = it.iterateNext();
    while (link) {
        var obj = new Object;
        obj.rel = link.rel;
        obj.type = link.type;
        obj.href = link.href;
        obj.title = link.title;
        links.push(obj);
        link = it.iterateNext();
    }
    return links;
})();