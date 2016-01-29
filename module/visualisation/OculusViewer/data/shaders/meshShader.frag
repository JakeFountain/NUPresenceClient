#version 150

in vec4 Pos;
in vec4 Normal;
in vec2 Texcoord;
out vec4 outColor;
uniform sampler2D tex;

int FORMAT_OTHER = 0;
int FORMAT_YCBCR_422 = 1;

uniform int format;
uniform int imageWidth;
uniform int imageHeight;


/**
 * A function for converting a YCbCr colour to RGBA
 * Based from http://en.wikipedia.org/wiki/YCbCr#JPEG_conversion
 *
 * @param {vec4} ycbcr A 4-component YCbCr array (includes alpha for convenience)
 * @returns {vec4} A converted RGBA colour (alpha untouched)
 */
 vec4 YCbCrToRGB(vec4 ycbcr) {
	// conversion numbers have been modified to account for the colour being in the 0-1 range instead of 0-255
	return clamp(vec4(
		ycbcr.r + 1.402 * (ycbcr.b - 128.0 / 255.0),
		ycbcr.r - 0.34414 * (ycbcr.g - 128.0 / 255.0) - 0.71414 * (ycbcr.b - 128.0 / 255.0),
		ycbcr.r + 1.772 * (ycbcr.g - 128.0 / 255.0),
		ycbcr.a
	), 0.0, 1.0);
}

vec4 textureYCbCr422(sampler2D rawImage, vec2 samplePoint) {
	float bytesPerPixel = 2.0;
	float rawImageWidth = bytesPerPixel * float(imageWidth);
	float startOffset = 0.5 / rawImageWidth;
	float texelSize = 1.0 / rawImageWidth;
	vec2 coord = samplePoint * vec2(float(imageWidth), float(imageHeight));

	vec2 yCoord = vec2(
		2.0 * texelSize * floor(coord.x) + startOffset,
		samplePoint.y
	);

	vec2 cbCoord = vec2(
		yCoord.x + texelSize * (1.0 - 2.0 * mod(floor(coord.x), 2.0)),
		yCoord.y
	);

	vec2 crCoord = vec2(
		yCoord.x + texelSize * (3.0 - 2.0 * mod(floor(coord.x), 2.0)),
		yCoord.y
	);

	float y = texture2D(rawImage, yCoord).r;
	float cb = texture2D(rawImage, cbCoord).r;
	float cr = texture2D(rawImage, crCoord).r;

	vec4 rawColour = vec4(y, cb, cr, 1.0);

	return rawColour;
}

void main() {
	if(format == FORMAT_YCBCR_422){
		outColor = YCbCrToRGB(textureYCbCr422(tex, vec2(Texcoord.x, 1-Texcoord.y)));
	} else {
		outColor = texture(tex, vec2(Texcoord.x, 1-Texcoord.y));
	}
	//outColor = vec4(Texcoord,0,1);
}