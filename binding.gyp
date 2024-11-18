{
  "targets": [
    {
      "target_name": "equihash",
      "sources": [
        "addon.cc",
        "pow.cc",
        "blake/blake2b.cpp"
      ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}
