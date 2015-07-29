
#include <Atomic/Resource/ResourceCache.h>
#include <Atomic/Resource/Image.h>

#include "Asset.h"
#include "AssetDatabase.h"
#include "TextureImporter.h"

namespace ToolCore
{

TextureImporter::TextureImporter(Context* context, Asset *asset) : AssetImporter(context, asset)
{

}

TextureImporter::~TextureImporter()
{

}

void TextureImporter::SetDefaults()
{
    AssetImporter::SetDefaults();
}

bool TextureImporter::Import(const String& guid)
{
    AssetDatabase* db = GetSubsystem<AssetDatabase>();
    Asset* asset = db->GetAssetByGUID(guid);

    if (!asset)
        return false;

    ResourceCache* cache = GetSubsystem<ResourceCache>();
    SharedPtr<Image> image = cache->GetTempResource<Image>(asset->GetPath());

    if (image.Null())
        return false;

    // todo, proper proportions
    image->Resize(64, 64);

    String cachePath = db->GetCachePath();

    // not sure entirely what we want to do here, though if the cache file doesn't exist
    // will reimport
    image->SavePNG(cachePath + asset->GetGUID());

    // save thumbnail
    image->SavePNG(cachePath + asset->GetGUID() + "_thumbnail.png");

    return true;
}

bool TextureImporter::LoadSettingsInternal()
{
    if (!AssetImporter::LoadSettingsInternal())
        return false;

    JSONValue import = jsonRoot_.GetChild("TextureImporter", JSON_OBJECT);

    return true;
}

bool TextureImporter::SaveSettingsInternal()
{
    if (!AssetImporter::SaveSettingsInternal())
        return false;

    JSONValue import = jsonRoot_.CreateChild("TextureImporter");

    return true;
}


}