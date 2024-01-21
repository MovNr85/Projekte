#define VOLK_IMPLEMENTATION

#include <iostream>
#include <vector>
#include <fstream>
#include <limits>

#include <assert.h>
#include <volk.h>
#include <GLFW/glfw3.h>


#define ASSERT_VULKAN(ret) \
    do { \
        VkResult result = ret; \
        assert(result == VK_SUCCESS); \
    } while (0)



uint32_t amount_of_images_in_swapchain = 0;






std::vector<char> readFile(const std::string& filename)
{
    std::ifstream file(filename, std::ios::binary | std::ios::ate);

    if (file)
    {
        size_t file_size = (size_t)file.tellg();
        std::vector<char> file_buffer(file_size);
        file.seekg(0);
        file.read(file_buffer.data(), file_size);
        file.close();
        return file_buffer;
    }
    else
    {
        throw std::runtime_error("Faild to open file!");
    }



}


VkInstance createVkInstance()
{


    VkApplicationInfo app_info;//{VK_STRUCTURE_TYPE_APPLICATION_INFO,0,"VulkanGraphics Test",VK_MAKE_VERSION(0,0,0),"Vulkan Test",VK_MAKE_VERSION(1,0,0),VK_API_VERSION_1_2};

    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pNext = nullptr;
    app_info.pApplicationName = "VulkanGraphics Test";
    app_info.applicationVersion = VK_MAKE_VERSION(0,0,0);
    app_info.pEngineName = "Vulkan Test";
    app_info.engineVersion = VK_MAKE_VERSION(1,0,0);
    app_info.apiVersion = VK_API_VERSION_1_2;



    const std::vector<const char*> validation_layers =
    {
        "VK_LAYER_KHRONOS_validation"
    };

    uint32_t amount_glfw_extensions = 0;
    auto glfwExtensions = glfwGetRequiredInstanceExtensions(&amount_glfw_extensions);

    VkInstanceCreateInfo app_create_info;
    app_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    app_create_info.pNext = nullptr;
    app_create_info.flags = 0;
    app_create_info.pApplicationInfo = &app_info;
    app_create_info.enabledLayerCount = validation_layers.size();
    app_create_info.ppEnabledLayerNames = validation_layers.data();
    app_create_info.enabledExtensionCount = amount_glfw_extensions;
    app_create_info.ppEnabledExtensionNames = glfwExtensions;

    VkInstance instance = 0;
    ASSERT_VULKAN(vkCreateInstance(&app_create_info, 0 , &instance));

    return instance;
}


/*
VkPhysicalDevice selectPhysicalVkDevice(VkPhysicalDevice* physical_devices, uint32_t physical_device_count)
{
    for (uint32_t i = 0; i < physical_device_count; i++)
    {
        VkPhysicalDeviceProperties props;
        vkGetPhysicalDeviceProperties(physical_devices[i], &props);

        if (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        {
            printf("Vulkan Device: %s\n", props.deviceName);
            return physical_devices[i];
        }
    }

    if (physical_device_count > 0)
    {
        VkPhysicalDeviceProperties props;
        vkGetPhysicalDeviceProperties(physical_devices[0], &props);

        printf("(Fallback) Vulkan Device: %s\n", props.deviceName);
        return physical_devices[0];
    }

    printf("No Physical Vulkan Device available !\n");
    return VK_NULL_HANDLE;

}*/


VkDevice createVkDevice(VkInstance instance, VkPhysicalDevice physical_device)
{

    /*
    uint32_t max_queue_families = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &max_queue_families, 0);
    VkQueueFamilyProperties* family_props = new VkQueueFamilyProperties[max_queue_families];
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &max_queue_families, family_props);
    */



    //float queue_props[] = {1.0f,1.0f,1.0f,1.0f};
    float queue_props[] = {1.0f};

    VkDeviceQueueCreateInfo queue_info; //{VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO};
    queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_info.pNext = nullptr;
    queue_info.flags = 0;
    queue_info.queueFamilyIndex = 0;
    queue_info.queueCount = 1;
    queue_info.pQueuePriorities = queue_props;

    const std::vector<const char*> device_extensions =
    {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    VkPhysicalDeviceFeatures device_features = {};

    VkDeviceCreateInfo device_info;  //{VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO};
    device_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    device_info.pNext = nullptr;
    device_info.flags = 0;
    device_info.queueCreateInfoCount = 1;
    device_info.pQueueCreateInfos = &queue_info;
    device_info.enabledLayerCount = 0;
    device_info.ppEnabledLayerNames = nullptr;
    device_info.enabledExtensionCount = device_extensions.size();
    device_info.ppEnabledExtensionNames = device_extensions.data();
    device_info.pEnabledFeatures = &device_features;


    VkDevice device = 0;
    //TODO pick "best device"
    ASSERT_VULKAN(vkCreateDevice(physical_device, &device_info, 0 , &device));
    assert(device);
    return device;
}


VkSwapchainKHR createVkSwapchain(VkDevice device, VkSurfaceKHR surface, uint32_t image_width, uint32_t image_height)
{


     VkSwapchainCreateInfoKHR swapchain_info = {VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR};
    //swapchain_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchain_info.pNext = nullptr;
    swapchain_info.flags = 0;
    swapchain_info.surface = surface;
    swapchain_info.minImageCount = 3;
    swapchain_info.imageFormat = VK_FORMAT_B8G8R8A8_UNORM;
    swapchain_info.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
    swapchain_info.imageExtent = VkExtent2D{image_width, image_height};
    swapchain_info.imageArrayLayers = 1;
    swapchain_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    swapchain_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    swapchain_info.queueFamilyIndexCount = 0;
    swapchain_info.pQueueFamilyIndices = nullptr;
    swapchain_info.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    swapchain_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapchain_info.presentMode = VK_PRESENT_MODE_FIFO_KHR;
    swapchain_info.clipped = VK_TRUE;
    swapchain_info.oldSwapchain = VK_NULL_HANDLE;


    VkSwapchainKHR swapchain = 0;
    ASSERT_VULKAN(vkCreateSwapchainKHR(device, &swapchain_info, 0, &swapchain));

    return swapchain;

}


VkImageView* createVkImageViews(VkDevice device, VkSwapchainKHR swapchain)
{

    vkGetSwapchainImagesKHR(device, swapchain, &amount_of_images_in_swapchain, nullptr);

    VkImage* swapchain_images = new VkImage[amount_of_images_in_swapchain];
    ASSERT_VULKAN(vkGetSwapchainImagesKHR(device, swapchain, &amount_of_images_in_swapchain, swapchain_images));

    VkImageView* image_views = new VkImageView[amount_of_images_in_swapchain];

    for (auto i = 0; i < amount_of_images_in_swapchain; i++)
    {
        VkImageViewCreateInfo image_view_create_info;
        image_view_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        image_view_create_info.pNext = nullptr;
        image_view_create_info.flags = 0;
        image_view_create_info.image = swapchain_images[i];
        image_view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
        image_view_create_info.format = VK_FORMAT_B8G8R8A8_UNORM;
        image_view_create_info.components =
        {
            VK_COMPONENT_SWIZZLE_IDENTITY,
            VK_COMPONENT_SWIZZLE_IDENTITY,
            VK_COMPONENT_SWIZZLE_IDENTITY,
            VK_COMPONENT_SWIZZLE_IDENTITY
        };
        image_view_create_info.subresourceRange =
        {
            VK_IMAGE_ASPECT_COLOR_BIT,
            0,
            1,
            0,
            1
        };


        ASSERT_VULKAN(vkCreateImageView(device, &image_view_create_info, nullptr,&image_views[i]));


    }

    return image_views;


}


VkShaderModule createVkShaderModule(VkDevice device, const std::string& shader_filename)
{

    auto shader_code = readFile(shader_filename);


    VkShaderModuleCreateInfo shader_create_info;
    shader_create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shader_create_info.pNext = nullptr;
    shader_create_info.flags = 0;
    shader_create_info.codeSize = shader_code.size();
    shader_create_info.pCode = reinterpret_cast<uint32_t*>(shader_code.data());

    printf("Shader Size: %d\n", shader_code.size());

    VkShaderModule shader_module = 0;
    ASSERT_VULKAN(vkCreateShaderModule(device, &shader_create_info, nullptr, &shader_module));

    return shader_module;

}




int main(int, char**)
{


    int rc = glfwInit();
    assert(rc);

    VkResult rt = volkInitialize();
    ASSERT_VULKAN(rt);


    if(glfwVulkanSupported())
    {
        printf("(GLFW) Vulkan Supported !\n");
    }





    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* app_window = 0;
    app_window = glfwCreateWindow(1024, 768, "VulkanGraphicsTest", 0, 0);
    assert(app_window);

    //Creating Vulkan Instance
    VkInstance app_instance = createVkInstance();


    volkLoadInstance(app_instance);


    //Creating Vulkan Surface
    VkSurfaceKHR app_surface = 0;
    glfwCreateWindowSurface(app_instance,app_window,nullptr,&app_surface);
    assert(app_surface);


    //Getting Physical Devices
    uint32_t physical_device_count = 0;
    ASSERT_VULKAN(vkEnumeratePhysicalDevices(app_instance, &physical_device_count, 0));

    VkPhysicalDevice* app_physical_devices = new VkPhysicalDevice[physical_device_count];
    assert(app_physical_devices);
    ASSERT_VULKAN(vkEnumeratePhysicalDevices(app_instance, &physical_device_count, app_physical_devices));

    VkBool32 physical_device_surface_support = 0;
    ASSERT_VULKAN(vkGetPhysicalDeviceSurfaceSupportKHR(app_physical_devices[0],0,app_surface,&physical_device_surface_support));
    assert(physical_device_surface_support);

    //Creating Vulkan Device
    VkDevice app_device = 0;
    app_device = createVkDevice(app_instance,app_physical_devices[0]);
    assert(app_device);

    volkLoadDevice(app_device);

    VkQueue app_queue = 0;
    vkGetDeviceQueue(app_device,0,0,&app_queue);



    //Setting Windows Width and Height
    uint32_t app_window_width = 1024;
    uint32_t app_window_height = 768;

    //glfwGetWindowSize(app_window, &app_window_width, &app_window_height);

    printf("Window Height: %d\n", app_window_height);
    printf("Window Width: %d\n", app_window_width);


    //Creating Vulkan Swapchain
    VkSwapchainKHR app_swapchain = createVkSwapchain(app_device,app_surface,app_window_width,app_window_height);


    //Creating Vulkan Image Views
    VkImageView* app_image_views = createVkImageViews(app_device, app_swapchain);



    auto app_shader_vert = createVkShaderModule(app_device, "vert.spv");
    auto app_shader_frag = createVkShaderModule(app_device,"frag.spv");



    VkPipelineShaderStageCreateInfo shader_stage_create_info_vert;
    shader_stage_create_info_vert.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shader_stage_create_info_vert.pNext = nullptr;
    shader_stage_create_info_vert.flags = 0;
    shader_stage_create_info_vert.stage = VK_SHADER_STAGE_VERTEX_BIT;
    shader_stage_create_info_vert.module = app_shader_vert;
    shader_stage_create_info_vert.pName = "main";
    shader_stage_create_info_vert.pSpecializationInfo = nullptr;



    VkPipelineShaderStageCreateInfo shader_stage_create_info_frag;
    shader_stage_create_info_frag.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shader_stage_create_info_frag.pNext = nullptr;
    shader_stage_create_info_frag.flags = 0;
    shader_stage_create_info_frag.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    shader_stage_create_info_frag.module = app_shader_frag;
    shader_stage_create_info_frag.pName = "main";
    shader_stage_create_info_frag.pSpecializationInfo = nullptr;

    VkPipelineShaderStageCreateInfo shader_stages[] = {shader_stage_create_info_vert, shader_stage_create_info_frag};


    VkPipelineVertexInputStateCreateInfo vertex_input_create_info;
    vertex_input_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertex_input_create_info.pNext = nullptr;
    vertex_input_create_info.flags = 0;
    vertex_input_create_info.vertexAttributeDescriptionCount = 0;
    vertex_input_create_info.pVertexBindingDescriptions = nullptr;
    vertex_input_create_info.pVertexAttributeDescriptions = nullptr;
    vertex_input_create_info.vertexBindingDescriptionCount = 0;


    VkPipelineInputAssemblyStateCreateInfo input_assembly_create_info;
    input_assembly_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    input_assembly_create_info.pNext = nullptr;
    input_assembly_create_info.flags = 0;
    input_assembly_create_info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    input_assembly_create_info.primitiveRestartEnable = VK_FALSE;

    VkViewport app_viewport;
    app_viewport.x = 0.0f;
    app_viewport.y = 0.0f;
    app_viewport.width = app_window_width;
    app_viewport.height = app_window_height;
    app_viewport.minDepth = 1.0f;


    VkRect2D app_scissor;
    app_scissor.offset = {0,0};
    app_scissor.extent = {app_window_width, app_window_height};


    VkPipelineViewportStateCreateInfo pipeline_viewport_state_create_info;
    pipeline_viewport_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    pipeline_viewport_state_create_info.pNext = nullptr;
    pipeline_viewport_state_create_info.flags = 0;
    pipeline_viewport_state_create_info.viewportCount = 1;
    pipeline_viewport_state_create_info.pViewports = &app_viewport;
    pipeline_viewport_state_create_info.scissorCount = 1;
    pipeline_viewport_state_create_info.pScissors = &app_scissor;


    VkPipelineRasterizationStateCreateInfo pipeline_rasterization_create_info;
    pipeline_rasterization_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    pipeline_rasterization_create_info.pNext = nullptr;
    pipeline_rasterization_create_info.flags = 0;
    pipeline_rasterization_create_info.depthClampEnable = VK_FALSE;
    pipeline_rasterization_create_info.rasterizerDiscardEnable = VK_FALSE;
    pipeline_rasterization_create_info.polygonMode = VK_POLYGON_MODE_FILL;
    pipeline_rasterization_create_info.cullMode = VK_CULL_MODE_BACK_BIT;
    pipeline_rasterization_create_info.frontFace = VK_FRONT_FACE_CLOCKWISE;
    pipeline_rasterization_create_info.depthBiasEnable = VK_FALSE;
    pipeline_rasterization_create_info.depthBiasClamp = 0.0f;
    pipeline_rasterization_create_info.depthBiasSlopeFactor = 0.0f;
    pipeline_rasterization_create_info.lineWidth = 1.0f;

    VkPipelineMultisampleStateCreateInfo pipeline_multisample_create_info;
    pipeline_multisample_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    pipeline_multisample_create_info.pNext = nullptr;
    pipeline_multisample_create_info.flags = 0;
    pipeline_multisample_create_info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    pipeline_multisample_create_info.sampleShadingEnable = VK_FALSE;
    pipeline_multisample_create_info.pSampleMask = nullptr;
    pipeline_multisample_create_info.alphaToCoverageEnable = VK_FALSE;
    pipeline_multisample_create_info.alphaToOneEnable = VK_FALSE;

    VkPipelineColorBlendAttachmentState pipeline_color_blend_attachment;
    pipeline_color_blend_attachment.blendEnable = VK_TRUE;
    pipeline_color_blend_attachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    pipeline_color_blend_attachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    pipeline_color_blend_attachment.colorBlendOp = VK_BLEND_OP_ADD;
    pipeline_color_blend_attachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    pipeline_color_blend_attachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    pipeline_color_blend_attachment.alphaBlendOp = VK_BLEND_OP_ADD;
    pipeline_color_blend_attachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;


    VkPipelineColorBlendStateCreateInfo pipeline_color_blend_state_create_info;
    pipeline_color_blend_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    pipeline_color_blend_state_create_info.pNext = nullptr;
    pipeline_color_blend_state_create_info.flags = 0;
    pipeline_color_blend_state_create_info.logicOpEnable = VK_FALSE;
    pipeline_color_blend_state_create_info.logicOp = VK_LOGIC_OP_NO_OP;
    pipeline_color_blend_state_create_info.attachmentCount = 1;
    pipeline_color_blend_state_create_info.pAttachments = &pipeline_color_blend_attachment;
    pipeline_color_blend_state_create_info.blendConstants[0] = 0.0f;
    pipeline_color_blend_state_create_info.blendConstants[1] = 0.0f;
    pipeline_color_blend_state_create_info.blendConstants[2] = 0.0f;
    pipeline_color_blend_state_create_info.blendConstants[3] = 0.0f;


    VkPipelineLayoutCreateInfo pipeline_layout_create_info;
    pipeline_layout_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipeline_layout_create_info.pNext = nullptr;
    pipeline_layout_create_info.flags = 0;
    pipeline_layout_create_info.setLayoutCount = 0;
    pipeline_layout_create_info.pSetLayouts = nullptr;
    pipeline_layout_create_info.pushConstantRangeCount = 0;
    pipeline_layout_create_info.pPushConstantRanges = nullptr;


    VkPipelineLayout app_pipeline_layout = 0;

    ASSERT_VULKAN(vkCreatePipelineLayout(app_device,&pipeline_layout_create_info,0,&app_pipeline_layout));

    VkAttachmentDescription attachment_description;
    attachment_description.flags = 0;
    attachment_description.format = VK_FORMAT_B8G8R8A8_UNORM;
    attachment_description.samples = VK_SAMPLE_COUNT_1_BIT;
    attachment_description.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachment_description.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachment_description.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachment_description.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachment_description.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attachment_description.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;


    VkAttachmentReference attachment_reference;
    attachment_reference.attachment = 0;
    attachment_reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass_description;
    subpass_description.flags = 0;
    subpass_description.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass_description.inputAttachmentCount = 0;
    subpass_description.pInputAttachments = nullptr;
    subpass_description.colorAttachmentCount = 1;
    subpass_description.pColorAttachments = &attachment_reference;
    subpass_description.pResolveAttachments = nullptr;
    subpass_description.pDepthStencilAttachment = nullptr;
    subpass_description.preserveAttachmentCount = 0;
    subpass_description.pPreserveAttachments = nullptr;


    VkSubpassDependency subpass_dependency;
    subpass_dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    subpass_dependency.dstSubpass = 0;
    subpass_dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    subpass_dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;;
    subpass_dependency.srcAccessMask = 0;
    subpass_dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    subpass_dependency.dependencyFlags = 0;

    VkRenderPassCreateInfo render_pass_create_info;
    render_pass_create_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    render_pass_create_info.pNext = nullptr;
    render_pass_create_info.flags = 0;
    render_pass_create_info.attachmentCount = 1;
    render_pass_create_info.pAttachments = &attachment_description;
    render_pass_create_info.subpassCount = 1;
    render_pass_create_info.pSubpasses = &subpass_description;
    render_pass_create_info.dependencyCount = 0;
    render_pass_create_info.pDependencies = &subpass_dependency;


    VkRenderPass app_render_pass = 0;
    ASSERT_VULKAN(vkCreateRenderPass(app_device, &render_pass_create_info, 0,&app_render_pass));
    printf("RenderPass: %u\n", app_render_pass);


    VkGraphicsPipelineCreateInfo graphics_pipeline_create_info;
    graphics_pipeline_create_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    graphics_pipeline_create_info.pNext = nullptr;
    graphics_pipeline_create_info.flags = 0;
    graphics_pipeline_create_info.stageCount = 2;
    graphics_pipeline_create_info.pStages = shader_stages;
    graphics_pipeline_create_info.pVertexInputState = &vertex_input_create_info;
    graphics_pipeline_create_info.pInputAssemblyState = &input_assembly_create_info;
    graphics_pipeline_create_info.pTessellationState = nullptr;
    graphics_pipeline_create_info.pViewportState = &pipeline_viewport_state_create_info;
    graphics_pipeline_create_info.pRasterizationState = &pipeline_rasterization_create_info;
    graphics_pipeline_create_info.pMultisampleState = &pipeline_multisample_create_info;
    graphics_pipeline_create_info.pDepthStencilState = nullptr;
    graphics_pipeline_create_info.pColorBlendState = &pipeline_color_blend_state_create_info;
    graphics_pipeline_create_info.pDynamicState = nullptr;
    graphics_pipeline_create_info.layout = app_pipeline_layout;
    graphics_pipeline_create_info.renderPass = app_render_pass;
    graphics_pipeline_create_info.subpass = 0;
    graphics_pipeline_create_info.basePipelineHandle = VK_NULL_HANDLE;
    graphics_pipeline_create_info.basePipelineIndex = -1;


    VkPipeline app_pipeline = 0;
    ASSERT_VULKAN(vkCreateGraphicsPipelines(app_device, VK_NULL_HANDLE,1,&graphics_pipeline_create_info,nullptr,&app_pipeline));
    printf("Pipeline: %u\n", app_pipeline);

    VkFramebuffer* app_framebuffers = new VkFramebuffer[amount_of_images_in_swapchain];

    for (auto i = 0; i < amount_of_images_in_swapchain; i++)
    {
        VkFramebufferCreateInfo framebuffer_create_info;
        framebuffer_create_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebuffer_create_info.pNext = nullptr;
        framebuffer_create_info.flags = 0;
        framebuffer_create_info.renderPass = app_render_pass;
        framebuffer_create_info.attachmentCount = 1;
        framebuffer_create_info.pAttachments = &(app_image_views[i]);
        framebuffer_create_info.width = app_window_width;
        framebuffer_create_info.height = app_window_height;
        framebuffer_create_info.layers = 1;

        ASSERT_VULKAN(vkCreateFramebuffer(app_device, &framebuffer_create_info, nullptr, &app_framebuffers[i]));



    }


    VkCommandPoolCreateInfo command_pool_create_info;
    command_pool_create_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    command_pool_create_info.pNext = nullptr;
    command_pool_create_info.flags = 0;
    command_pool_create_info.queueFamilyIndex = 0;

    VkCommandPool app_command_pool = 0;
    ASSERT_VULKAN(vkCreateCommandPool(app_device,&command_pool_create_info, nullptr, &app_command_pool));


    VkCommandBufferAllocateInfo command_buffer_allocate_info;
    command_buffer_allocate_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    command_buffer_allocate_info.pNext = nullptr;
    command_buffer_allocate_info.commandPool = app_command_pool;
    command_buffer_allocate_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    command_buffer_allocate_info.commandBufferCount = amount_of_images_in_swapchain;



    VkCommandBuffer* app_command_buffers = new VkCommandBuffer[amount_of_images_in_swapchain];
    ASSERT_VULKAN(vkAllocateCommandBuffers(app_device,&command_buffer_allocate_info,app_command_buffers));


    VkCommandBufferBeginInfo command_buffer_begin_info;
    command_buffer_begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    command_buffer_begin_info.pNext = nullptr;
    command_buffer_begin_info.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
    command_buffer_begin_info.pInheritanceInfo = nullptr;


    for (auto i = 0; i < amount_of_images_in_swapchain; i++)
    {
        ASSERT_VULKAN(vkBeginCommandBuffer(app_command_buffers[i], &command_buffer_begin_info));


        VkRenderPassBeginInfo render_pass_begin_info;
        render_pass_begin_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        render_pass_begin_info.pNext = nullptr;
        render_pass_begin_info.renderPass = app_render_pass;
        render_pass_begin_info.framebuffer = app_framebuffers[i];
        render_pass_begin_info.renderArea.offset = {0,0};
        render_pass_begin_info.renderArea.extent = {app_window_width, app_window_height};

        VkClearValue clear_value = {{{0.0f,0.0f,0.0f,1.0f}}};
        render_pass_begin_info.clearValueCount = 1;
        render_pass_begin_info.pClearValues = &clear_value;


        vkCmdBeginRenderPass (app_command_buffers[i], &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(app_command_buffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, app_pipeline);

        vkCmdDraw(app_command_buffers[i], 3, 1,0,0);


        vkCmdEndRenderPass(app_command_buffers[i]);


        ASSERT_VULKAN(vkEndCommandBuffer(app_command_buffers[i]));
    }



    VkSemaphore app_semaphore_image_available = 0;

    VkSemaphore app_semaphore_rendering_done = 0;


    VkSemaphoreCreateInfo semaphore_create_info;
    semaphore_create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphore_create_info.pNext = nullptr;
    semaphore_create_info.flags = 0;

    ASSERT_VULKAN(vkCreateSemaphore(app_device, &semaphore_create_info, nullptr, &app_semaphore_image_available));
    ASSERT_VULKAN(vkCreateSemaphore(app_device, &semaphore_create_info, nullptr, &app_semaphore_rendering_done));






    while (!glfwWindowShouldClose(app_window))
    {

        glfwPollEvents();


        //Drawing Frames



        uint32_t image_index;
        vkAcquireNextImageKHR(app_device,app_swapchain,std::numeric_limits<uint64_t>::max(), app_semaphore_image_available,VK_NULL_HANDLE,&image_index);

        VkSubmitInfo submit_info;
        submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submit_info.pNext = nullptr;
        submit_info.waitSemaphoreCount = 1;
        submit_info.pWaitSemaphores = &app_semaphore_image_available;
        VkPipelineStageFlags wait_stage_mask[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submit_info.pWaitDstStageMask = wait_stage_mask;
        submit_info.commandBufferCount = 1;
        submit_info.pCommandBuffers = &app_command_buffers[image_index];
        submit_info.signalSemaphoreCount = 1;
        submit_info.pSignalSemaphores = &app_semaphore_rendering_done;

        ASSERT_VULKAN(vkQueueSubmit(app_queue,1,&submit_info, VK_NULL_HANDLE));

        VkPresentInfoKHR present_info;
        present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        present_info.pNext = nullptr;
        present_info.waitSemaphoreCount = 1;
        present_info.pWaitSemaphores = &app_semaphore_rendering_done;
        present_info.swapchainCount = 1;
        present_info.pSwapchains = &app_swapchain;
        present_info.pImageIndices = &image_index;
        present_info.pResults = nullptr;

        ASSERT_VULKAN(vkQueuePresentKHR(app_queue, &present_info));


    }
    printf("Destroying Window !\n");
    glfwDestroyWindow(app_window);


    vkDeviceWaitIdle(app_device);



    vkDestroySemaphore(app_device, app_semaphore_image_available, nullptr);
    vkDestroySemaphore(app_device,app_semaphore_rendering_done, nullptr);

    for (auto i = 0; i < amount_of_images_in_swapchain; i++)
    {
        vkDestroyFramebuffer(app_device,app_framebuffers[i], nullptr);
    }

    vkFreeCommandBuffers(app_device,app_command_pool, amount_of_images_in_swapchain, app_command_buffers);

    delete[] app_command_buffers;

    vkDestroyCommandPool(app_device, app_command_pool, nullptr);

    vkDestroyPipeline(app_device,app_pipeline, nullptr);

    vkDestroyRenderPass(app_device,app_render_pass, nullptr);

    //TODO Gllobale Variable Handeln !
    for (auto i = 0; i < amount_of_images_in_swapchain; i++)
    {
        vkDestroyImageView(app_device, app_image_views[i], nullptr);
    }

    delete[] app_image_views;
    vkDestroyPipelineLayout(app_device,app_pipeline_layout,nullptr);
    vkDestroyShaderModule(app_device, app_shader_vert,nullptr);
    vkDestroyShaderModule(app_device, app_shader_frag, nullptr);

    vkDestroySwapchainKHR(app_device, app_swapchain, nullptr);
    vkDestroyDevice(app_device, nullptr);
    vkDestroySurfaceKHR(app_instance, app_surface, nullptr);
    vkDestroyInstance(app_instance, 0);

}
