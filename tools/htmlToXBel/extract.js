
function walk() {
    var parent = arguments[0];
    var indent = arguments[1];

    var result = "";
    var children = parent.childNodes;
    var folderName = "";
    var folded = "";
    for (var i = 0; i < children.length; i++) {
        var object = children.item(i);
        if (object.nodeName == "HR") {
            result += indent + "<separator/>\n";
        }
        if (object.nodeName == "H3") {
            folderName = object.innerHTML;
            folded = object.folded;
            if (object.folded == undefined)
                folded = "false";
            else
                folded = "true";
        }
        if (object.nodeName == "A") {
            result += indent + "<bookmark href=\"" + encodeURI(object.href).replace(/&/g, '&amp;') + "\">\n";
            result += indent + indent + "<title>" + object.innerHTML + "</title>\n";
            result += indent + "</bookmark>\n";
        }

        var currentIndent = indent;
        if (object.nodeName == "DL") {
            result += indent + "<folder folded=\"" + folded + "\">\n";
            indent += "    ";
            result += indent + "<title>" + folderName + "</title>\n";
        }
        result += walk(object, indent);
        if (object.nodeName == "DL") {
            result += currentIndent + "</folder>\n";
        }
    }
    return result;
}

var xbel = walk(document, "    ");

if (xbel != "") {
    xbel = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<!DOCTYPE xbel>\n<xbel version=\"1.0\">\n" + xbel + "</xbel>\n";
}

xbel;
