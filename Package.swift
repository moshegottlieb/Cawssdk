// swift-tools-version:4.0
// The swift-tools-version declares the minimum version of Swift required to build this package.

import PackageDescription

let package = Package(
    name: "Cawssdk",
    products: [
        .library(
            name: "Cawssdk",
            targets: ["Cawssdk"]),
        ],
    dependencies: [
    ],
    targets: [
        .target(
            name: "Cawssdk",
            dependencies: []),
        ]
)
